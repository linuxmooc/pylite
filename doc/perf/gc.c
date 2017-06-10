#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "gc.h"

void *gc_memory_low;
void *gc_memory_high;
int gc_memory_size;

void *gc_object_list;
void *gc_special_list;

pthread_spinlock_t gc_mutex;
static inline void gc_lock()
{
    pthread_spin_lock(&gc_mutex);
}

static inline void gc_unlock()
{
    pthread_spin_unlock(&gc_mutex);
}

static inline void gc_free_object(py_object_t *py_object)
{
    py_object->next = gc_object_list;
    gc_object_list = py_object;
}

static inline void gc_free_special(py_object_t *py_special)
{
    py_special->next = gc_special_list;
    gc_special_list = py_special;
}

void gc_setup(int object_count, int special_count)
{
    gc_memory_size = object_count * GC_OBJECT_SIZE + special_count * GC_SPECIAL_SIZE;

    void *address;
    int error = posix_memalign(&address, CACHE_SIZE, gc_memory_size);
    if (error != 0) {
        fprintf(stderr, "gc_setup failed!\n");
        exit(EXIT_FAILURE);
    }

    gc_memory_low = address;
    gc_memory_high = address + gc_memory_size;

    gc_object_list = gc_memory_low;
    gc_special_list = gc_memory_low + object_count * GC_OBJECT_SIZE;

    void *gc_object = gc_memory_low; 
    for (; gc_object < gc_special_list; gc_object += GC_OBJECT_SIZE)
        gc_free_object(gc_object);
    assert(gc_object == gc_special_list);

    void *gc_special = gc_special_list;
    for (; gc_special < gc_memory_high; gc_special += GC_SPECIAL_SIZE)
        gc_free_special(gc_special);
    assert(gc_special == gc_memory_high);

    pthread_spin_init(&gc_mutex, PTHREAD_PROCESS_PRIVATE);
}

py_object_t *gc_alloc_object(int size)
{
    assert(size <= GC_OBJECT_SIZE);

    gc_lock();
    py_object_t *py_object = gc_object_list;
    assert(py_object != NULL);
    gc_object_list = ((py_object_t *)gc_object_list)->next;
    gc_unlock();

    return py_object;
}

py_object_t *gc_alloc_special(int size)
{
    assert(size < GC_SPECIAL_SIZE);

    py_object_t *py_special = gc_special_list;
    assert(py_special != NULL);
    gc_special_list = ((py_object_t *)gc_special_list)->next;
    return py_special;
}

void *global;
void test_gc_alloc(int n)
{
    int i;
    gc_setup(n, 1024);
    for (i = 0; i < n; i++)
        global = gc_alloc_object(GC_OBJECT_SIZE);
    exit(0);
}

void test_malloc(int n)
{
    int i;
    for (i = 0; i < n; i++)
        global = malloc(GC_OBJECT_SIZE);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc > 1 && (strcmp(argv[1], "gc_alloc") == 0))
        test_gc_alloc(1024 * 1024);

    if (argc > 1 && (strcmp(argv[1], "malloc") == 0))
        test_malloc(1024 * 1024);

    fprintf(stderr, "Usage: gc [gc_alloc|malloc]\n");
    return 0;
}
