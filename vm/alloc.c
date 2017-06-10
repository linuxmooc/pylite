#include "root.h"
#include "gc.h"
#include "alloc.h"

void *gc_memory_low;
void *gc_memory_mid;
void *gc_memory_high;
int gc_memory_size;

gc_pool_t gc_small_pool;
gc_pool_t gc_large_pool;
void *gc_object_list;

bool gc_guess_object(void *p)
{
    if (gc_memory_low <= p && p < gc_memory_mid) {
        unsigned long offset = (unsigned long)(p - gc_memory_low);
        if (offset % GC_SMALL_SIZE != 0)
            return false;
        py_object_t *py_object = p;
        return (py_object->flags & GC_MAGIC) == GC_LIVE_MAGIC;
    }

    if (gc_memory_mid <= p && p < gc_memory_high) {
        unsigned long offset = (unsigned long)(p - gc_memory_mid);
        if (offset % GC_LARGE_SIZE != 0)
            return false;
        py_object_t *py_object = p;
        return (py_object->flags & GC_MAGIC) == GC_LIVE_MAGIC;
    }

    return false;
}

static inline void *gc_pool_alloc(gc_pool_t *this, int object_size)
{
    assert(object_size <= this->object_size);

    py_object_t *py_object = this->object_list;
    if (py_object == NULL) {
        return NULL;
    }
    this->object_list = ((py_object_t *)this->object_list)->next;
    py_object->flags = GC_LIVE_MAGIC;
    py_object->next = gc_object_list;
    gc_object_list = py_object; 

    return py_object;
}

static inline void gc_pool_free(gc_pool_t *this, py_object_t *py_object)
{
#ifndef NDEBUG
    memset(py_object, 0, this->object_size);
#endif
    py_object->flags = GC_DEAD_MAGIC;
    py_object->next = this->object_list;
    this->object_list = py_object;
}

void gc_pool_init(gc_pool_t *this, int object_size, 
                  void *memory_low, void *memory_high)
{
    this->object_size = object_size;
    this->memory_low = memory_low;
    this->memory_high = memory_high;

    void *py_object = memory_low; 
    for (; py_object < memory_high; py_object += object_size)
        gc_pool_free(this, py_object);
}

py_object_t *gc_alloc_object(int size)
{
    py_object_t *py_object;

    if (size <= GC_SMALL_SIZE)
        py_object = gc_pool_alloc(&gc_small_pool, size);
    else
        py_object = gc_pool_alloc(&gc_large_pool, size);
    if (py_object != NULL)
        return py_object;

    gc_start();
    if (size <= GC_SMALL_SIZE)
        py_object = gc_pool_alloc(&gc_small_pool, size);
    else
        py_object = gc_pool_alloc(&gc_large_pool, size);
    if (py_object == NULL) {
        fprintf(stderr, "gc failed!\n");
        exit(EXIT_FAILURE);
    }
    return py_object;
}

void gc_free_object(py_object_t *py_object)
{
    py_class_t *py_class = py_object->py_class;
    if (py_class && py_class->gc_free)
        py_class->gc_free(py_object);

    void *data = py_object->field_vector.data;
    if (data != NULL)
        free(data);

    if (((void *)py_object) < gc_memory_mid)
        gc_pool_free(&gc_small_pool, py_object);
    else
        gc_pool_free(&gc_large_pool, py_object);
}

void gc_setup(int object_count)
{
    int small_count = object_count * 15 / 16;
    int large_count = object_count * 1 / 16;
    gc_memory_size = small_count * GC_SMALL_SIZE + large_count * GC_LARGE_SIZE;

    void *address;
    int error = posix_memalign(&address, CACHE_SIZE, gc_memory_size);
    if (error != 0) {
        fprintf(stderr, "gc_setup failed!\n");
        exit(EXIT_FAILURE);
    }

    gc_memory_low = address;
    gc_memory_mid = address + small_count * GC_SMALL_SIZE;
    gc_memory_high = address + gc_memory_size;

    gc_object_list = NULL;
    gc_pool_init(&gc_small_pool, GC_SMALL_SIZE, gc_memory_low, gc_memory_mid);
    gc_pool_init(&gc_large_pool, GC_LARGE_SIZE, gc_memory_mid, gc_memory_high);
}
