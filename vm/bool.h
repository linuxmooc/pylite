#ifndef _PY_BOOL_H
#define _PY_BOOL_H

#include "object.h"
#include "class.h"

extern py_object_t *py_true;
extern py_object_t *py_false;
extern py_class_t *py_bool_class;
extern void py_bool_class_init(void);
extern void py_bool_init(void);

#endif
