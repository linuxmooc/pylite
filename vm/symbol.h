#ifndef _PY_SYMBOL_H
#define _PY_SYMBOL_H

#include "object.h"
#include "class.h"

typedef struct py_symbol {
    py_object_member_list;
    char *value;
} py_symbol_t;

extern py_symbol_t *py_symbol_new(char *value);
extern py_class_t *py_symbol_class;
extern void py_symbol_class_init(void);
extern void py_symbol_vector_init(void);

extern py_symbol_t *py_symbol__init__;
extern py_symbol_t *py_symbol__add__;
extern py_symbol_t *py_symbol__sub__;
extern py_symbol_t *py_symbol__mul__;
extern py_symbol_t *py_symbol__div__;
extern py_symbol_t *py_symbol__mod__;

extern py_symbol_t *py_symbol__le__;
extern py_symbol_t *py_symbol__lt__;
extern py_symbol_t *py_symbol__ne__;
extern py_symbol_t *py_symbol__eq__;
extern py_symbol_t *py_symbol__ge__;
extern py_symbol_t *py_symbol__gt__;

extern py_symbol_t *py_symbol__len__;
extern py_symbol_t *py_symbol__str__;
extern py_symbol_t *py_symbol__getitem__;
extern py_symbol_t *py_symbol__setitem__;
extern py_symbol_t *py_symbol__iter__;
extern py_symbol_t *py_symbol__next__;

extern py_symbol_t *py_symbol_start;
extern py_symbol_t *py_symbol_stop;
extern py_symbol_t *py_symbol_join;
extern py_symbol_t *py_symbol_run;

#endif
