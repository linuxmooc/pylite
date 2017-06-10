#ifndef _VM_GC_H
#define _VM_GC_H

typedef struct py_object {
    struct py_object *next;
} py_object_t;

#define GC_PINNED 0x1
#define GC_REACHABLE 0x2

extern void gc_setup(int object_count, int special_count);
extern py_object_t *gc_alloc_object(int size);
extern py_object_t *gc_alloc_special(int size);

#define CACHE_SIZE 64
#define GC_OBJECT_SIZE CACHE_SIZE
#define GC_SPECIAL_SIZE 256
static inline py_object_t *gc_alloc(int size)
{
    if (size <= GC_OBJECT_SIZE)
        return gc_alloc_object(size);
    else
        return gc_alloc_special(size);
}

#endif
