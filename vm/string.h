#ifndef _PY_STRING_H
#define _PY_STRING_H

#include "object.h"
#include "class.h"

typedef struct py_string {
    py_object_member_list;
    char *value;
} py_string_t;

extern py_class_t *py_string_class;
extern void py_string_class_init(void);
extern py_string_t *py_string_new(char *value);

static inline char *py_object_to_chars(py_object_t *this)
{
    assert(this->py_class == py_string_class);
    py_string_t *py_string = (py_string_t *)this;
    return py_string->value;
}

#endif
