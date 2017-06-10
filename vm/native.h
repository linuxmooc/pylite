#ifndef _PY_NATIVE_H
#define _PY_NATIVE_H

#include "object.h"
#include "class.h"
#include "symbol.h"

typedef py_object_t* (*handler_t)(int argc, py_object_t *argv[]);

typedef struct native {
    char *name;
    handler_t handler;
} native_t;

typedef struct {
    py_object_member_list;
    py_symbol_t *name;
    handler_t handler;
} py_native_t;

extern py_class_t *py_native_class;
extern void py_native_class_init(void);
extern py_native_t *py_native_new(py_symbol_t *name, handler_t handler);

#endif
