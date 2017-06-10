#ifndef _PY_NONE_H
#define _PY_NONE_H

#include "object.h"
#include "class.h"

extern py_object_t *py_none;
extern py_class_t *py_none_class;
extern void py_none_class_init();
extern void py_none_init();

#endif
