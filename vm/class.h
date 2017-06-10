#ifndef _PY_ClASS_H
#define _PY_ClASS_H

#include "object.h"
struct native;
struct py_symbol;
struct py_class;

typedef py_object_t *(*py_alloc_t)(struct py_class *);

typedef struct py_class {
    py_object_member_list;
    char *name;
    py_alloc_t py_alloc;
    void (*gc_mark)(py_object_t *);
    void (*gc_free)(py_object_t *);
    struct py_class *parent;
} py_class_t;

extern py_class_t *py_class_new(char *name);
extern bool py_class_is_subclass(py_class_t *this, py_class_t *parent);
extern py_object_t *py_class_load_field(py_class_t *this, 
                                        struct py_symbol *name);
extern void py_class_register_natives(py_class_t *this,
                                      struct native *natives);
#endif
