#ifndef _PY_TUPLE_H
#define _PY_TUPLE_H

#include "object.h"
#include "class.h"

typedef struct py_tuple {
    py_object_member_list;
    vector_template(py_object_t *, vector);
    int cursor;
} py_tuple_t;

extern py_class_t *py_tuple_class;
extern void py_tuple_class_init(void);
extern py_tuple_t *py_tuple_new();
extern void py_tuple_append(py_tuple_t *this, py_object_t *py_item);
extern py_object_t *py_tuple_get_item(int argc, py_object_t *argv[]);

#endif
