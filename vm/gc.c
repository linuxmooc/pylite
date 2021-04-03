#include "root.h"
#include "gc.h"
#include "alloc.h"
#include "object.h"
#include "vm.h"
#include "double.h"
#include "module.h"
#include <setjmp.h>

#define DEBUG_GC 0

void *c_stack_low;
void *c_stack_high;

vector_template(py_object_t *, gc_root_vector);

void gc_add_root(py_object_t *py_object)
{
    vector_push_back(&gc_root_vector, py_object);
}

void __gc_mark(py_object_t *this)
{
    if (this->flags & GC_REACHABLE)
        return;
    this->flags |= GC_REACHABLE;

    py_class_t *py_class = this->py_class;
    if (py_class == py_double_class)
        return;

    int i;
    field_t *field;
    vector_each_address (&this->field_vector, i, field) {
        py_object_t *child = field->py_value;
        if ((child->flags & GC_REACHABLE) == 0)
            gc_mark(child);
    }

    if (py_class && py_class->gc_mark)
        py_class->gc_mark(this);
}

void gc_found()
{
#if DEBUG_GC
    int sum = 0;
    py_object_t *py_object = gc_object_list; 
    for (; py_object != NULL; py_object = py_object->next) {
        if (py_object->flags & GC_REACHABLE)
            sum++;
    }
    printf("Number of reach = %d\n", sum);
#endif
}

void gc_sweep()
{
    int sum = 0;
    void *temp_list = NULL;

    py_object_t *py_object = gc_object_list; 
    py_object_t* py_next;
    while (py_object != NULL) {
        py_next = py_object->next;
        if (py_object->flags & GC_REACHABLE) {
            py_object->flags &= ~GC_REACHABLE;
            py_object->next = temp_list;
            temp_list = py_object;
        } else {
            sum++;
            gc_free_object(py_object);
        }
        py_object = py_next;
    }

#if DEBUG_GC
    printf("Number of sweep = %d\n", sum);
#endif
    gc_object_list = temp_list;
}

void gc_mark_root()
{
    int i;
    py_object_t *py_object;

    vector_each (&gc_root_vector, i, py_object) {
        gc_mark(py_object);
    }
}

void gc_mark_c_stack()
{
    jmp_buf context;
    setjmp(context);
    c_stack_low = &context;

    for (void **slot = c_stack_low; (void *)slot < c_stack_high; slot++) {
        void *p = *slot;
        if (gc_guess_object(p))
            gc_mark(p);
    }
}    

void gc_mark_vm_stack()
{
    for (int i = sp; i < VM_STACK_DEPTH; i++)
        gc_mark(vm_stack[i]);
}

void gc_start()
{
    gc_mark_root();
    gc_mark_vm_stack();
    gc_mark_c_stack();
    gc_found();
    gc_sweep();
}
