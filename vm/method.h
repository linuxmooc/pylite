#ifndef _PY_METHOD_H
#define _PY_METHOD_H

#include "object.h"
#include "class.h"

typedef struct py_method {
    py_object_member_list;
    py_object_t *py_object;
    py_object_t *py_callable;
} py_method_t;

extern py_class_t *py_method_class;
extern void py_method_class_init();
extern py_method_t *py_method_new(py_object_t *py_object, 
                                  py_object_t *py_callable);
#endif
