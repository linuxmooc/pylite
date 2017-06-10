#ifndef _PY_MODULE_H
#define _PY_MODULE_H

#include "object.h"
#include "class.h"
#include "symbol.h"
#include "native.h"

typedef struct py_module {
    py_object_member_list;
    char *dir;
    char *name;
} py_module_t;

extern void py_module_vector_init();
extern py_module_t *py_module_new(char *dir, char *name);
extern void register_py_module(py_module_t *py_module);
extern void open_main_module(char *path);
extern py_module_t *load_py_module(char *dir, char *name);

extern void py_module_import_object(py_module_t *this, char *name, 
                                    py_object_t *py_value);
extern void py_module_import_double(py_module_t *this, char *name, 
                                    double value);
extern void py_module_import_string(py_module_t *this, char *name, 
                                    char *value);
extern void py_module_import_lambda(py_module_t *this, char *name, 
                                    handler_t value);
extern void py_module_import_class(py_module_t *this, char *name, 
                                   py_class_t *value);
extern void py_module_import_all(py_module_t *this, py_module_t *that);
#endif
