#ifndef _VM_GC_H
#define _VM_GC_H

#include "object.h"
#define C_STACK_DEPTH 65536
extern py_object_t *c_stack[C_STACK_DEPTH];
extern int cp;

static inline void c_stack_push(void *py_object)
{
    c_stack[--cp] = py_object;
}

static inline py_object_t *c_stack_pop()
{
    return c_stack[cp++];
}

static inline int c_stack_save()
{
    return cp;
}

static inline void c_stack_restore(int old_cp)
{
    cp = old_cp;
}

int cp = C_STACK_DEPTH;

#endif
