#ifndef _VM_VM_H
#define _VM_VM_H

#include "object.h"
#include "class.h"
#include "lambda.h"
#include "method.h"

#define VM_STACK_DEPTH 65536
extern py_object_t *vm_stack[VM_STACK_DEPTH];
extern int sp;

static inline void *vm_stack_top(int offset)
{
    return vm_stack[sp + offset];
}

static inline void vm_stack_push(void *py_object)
{
    vm_stack[--sp] = py_object;
}

static inline void *vm_stack_pop()
{
    return vm_stack[sp++];
}

static inline void vm_stack_eject(int count)
{
    sp += count;
}

#define arguments int argc, py_object_t *argv[]
extern py_object_t *vm_call(py_object_t *py_callable, arguments);
extern py_object_t *vm_call_lambda(py_lambda_t *py_lambda, arguments);

#endif
