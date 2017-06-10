#ifndef _PY_LIST_H
#define _PY_LIST_H

#include "object.h"
#include "class.h"

typedef struct py_list {
    py_object_member_list;
    vector_template(py_object_t *, vector);
    int cursor;
} py_list_t;

extern py_class_t *py_list_class;
extern void py_list_class_init(void);
extern py_list_t *py_list_new(int count);
extern void py_list_push_back(py_list_t *this, py_object_t *py_item);
extern py_object_t *py_list_get_item(int argc, py_object_t *argv[]);
extern py_object_t *py_list_set_item(int argc, py_object_t *argv[]);

#endif
