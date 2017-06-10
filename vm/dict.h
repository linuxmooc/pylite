#ifndef _PY_DICT_H
#define _PY_DICT_H

#include "object.h"
#include "class.h"

typedef struct {
    py_object_t *key;
    py_object_t *value;
} py_pair_t;

typedef struct py_dict {
    py_object_member_list;
    vector_template(py_pair_t, vector);
} py_dict_t;

extern py_class_t *py_dict_class;
extern void py_dict_class_init(void);
extern py_dict_t *py_dict_new();
extern py_object_t *py_dict_get_item(int argc, py_object_t *argv[]);
extern py_object_t *py_dict_set_item(int argc, py_object_t *argv[]);

#endif
