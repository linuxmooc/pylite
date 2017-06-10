#ifndef _VM_GC_H
#define _VM_GC_H

#include "object.h"
#include "class.h"
#include "double.h"

#define GC_MAGIC 0xFFFFFF00
#define GC_DEAD_MAGIC 0xDEAD9800
#define GC_LIVE_MAGIC 0x115E9700
#define GC_REACHABLE 0x1

extern void __gc_mark(py_object_t *this);
static inline void gc_mark(py_object_t *this)
{
    if (this->py_class == py_double_class) {
        this->flags |= GC_REACHABLE;
        return;
    }

    __gc_mark(this);
}
extern void gc_sweep();
extern void gc_start();

#define CACHE_SIZE 64
#define GC_SMALL_SIZE CACHE_SIZE

#ifdef __LP64__
#define GC_LARGE_SIZE (CACHE_SIZE * 4)
#else
#define GC_LARGE_SIZE (CACHE_SIZE * 2)
#endif

extern void *c_stack_low;
extern void *c_stack_high;
extern void gc_add_root(py_object_t *py_object);

#endif
