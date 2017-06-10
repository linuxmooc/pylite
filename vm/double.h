#ifndef _PY_DOUBLE_H
#define _PY_DOUBLE_H

#include "object.h"
#include "class.h"

typedef struct py_double {
    py_object_member_list;
    double value;
} py_double_t;

#define py_object_is_double(py_object) (py_object->py_class == py_double_class)
extern struct py_class *py_double_class;
extern void py_double_class_init();
static inline py_double_t *py_double_new(double value)
{
    py_double_t *this = py_object_alloc(sizeof(py_double_t), py_double_class);
    this->value = value;
    return this;
}

static inline double py_object_to_double(py_object_t *this)
{
    assert(this->py_class == py_double_class);
    py_double_t *py_double = (py_double_t *)this;
    return py_double->value;
}

static inline int py_object_to_integer(py_object_t *this)
{
    assert(this->py_class == py_double_class);
    py_double_t *py_double = (py_double_t *)this;
    return (int) (py_double->value);
}

#endif
