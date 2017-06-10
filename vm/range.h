#ifndef _PY_RANGE_H
#define _PY_RANGE_H

#include "object.h"
#include "class.h"

typedef struct py_range {
    py_object_member_list;
    int start;
    int end;
    int step;
    int cursor;
} py_range_t;

extern py_class_t *py_range_class;
extern void py_range_class_init();

#endif
