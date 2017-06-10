#ifndef _PY_SLICE_H
#define _PY_SLICE_H

#include "object.h"
#include "class.h"

typedef struct py_slice {
    py_object_member_list;
    py_object_t *start;
    py_object_t *stop;
} py_slice_t;

extern py_class_t *py_slice_class;
extern void py_slice_class_init();
extern py_slice_t *py_slice_new(int argc, py_object_t *argv[]);
extern py_object_t *py_slice_parse(py_slice_t *this, 
                                   int *start, int *stop, int size);

#endif
