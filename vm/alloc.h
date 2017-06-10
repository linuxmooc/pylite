#ifndef _VM_ALLOC_H
#define _VM_ALLOC_H

#include <pthread.h>

extern void *gc_memory_low;
extern void *gc_memory_mid;
extern void *gc_memory_high;
extern int gc_memory_size;
extern void gc_setup(int object_count);

typedef struct {
    int object_size;
    void *memory_low;
    void *memory_high;
    void *object_list;
} gc_pool_t;

extern gc_pool_t gc_small_pool;
extern gc_pool_t gc_large_pool;
extern pthread_spinlock_t gc_mutex;
extern void *gc_object_list;

extern py_object_t *gc_alloc_object(int size);
extern void gc_free_object(py_object_t *py_object);
extern bool gc_guess_object(void *p);

#endif
