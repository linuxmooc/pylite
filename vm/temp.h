#ifndef _PY_TEMP_H
#define _PY_TEMP_H

#include "object.h"
#include "class.h"

typedef struct py_temp {
    py_object_member_list;
} py_temp_t;

extern py_class_t *py_temp_class;
extern py_temp_t *py_temp_new();
extern void py_temp_class_init(void);

#endif
