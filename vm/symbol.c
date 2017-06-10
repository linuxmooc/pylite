#include "root.h"
#include "symbol.h"

py_class_t *py_symbol_class;

vector_template(py_symbol_t *, py_symbol_vector);
py_symbol_t *py_symbol__init__;
py_symbol_t *py_symbol__add__;
py_symbol_t *py_symbol__sub__;
py_symbol_t *py_symbol__mul__;
py_symbol_t *py_symbol__div__;
py_symbol_t *py_symbol__mod__;

py_symbol_t *py_symbol__le__;
py_symbol_t *py_symbol__lt__;
py_symbol_t *py_symbol__ne__;
py_symbol_t *py_symbol__eq__;
py_symbol_t *py_symbol__ge__;
py_symbol_t *py_symbol__gt__;

py_symbol_t *py_symbol__len__;
py_symbol_t *py_symbol__str__;
py_symbol_t *py_symbol__getitem__;
py_symbol_t *py_symbol__setitem__;
py_symbol_t *py_symbol__iter__;
py_symbol_t *py_symbol__next__;

py_symbol_t *py_symbol_start;
py_symbol_t *py_symbol_stop;
py_symbol_t *py_symbol_join;
py_symbol_t *py_symbol_run;

void py_symbol_vector_init(void)
{
    vector_init(&py_symbol_vector);
}    

void py_symbol_class_init(void)
{
    py_symbol_class = py_class_new("symbol");

    py_symbol__init__ = py_symbol_new("__init__");
    py_symbol__add__ = py_symbol_new("__add__");
    py_symbol__sub__ = py_symbol_new("__sub__");
    py_symbol__mul__ = py_symbol_new("__mul__");
    py_symbol__div__ = py_symbol_new("__div__");
    py_symbol__mod__ = py_symbol_new("__mod__");

    py_symbol__lt__ = py_symbol_new("__lt__");
    py_symbol__le__ = py_symbol_new("__le__");
    py_symbol__ne__ = py_symbol_new("__ne__");
    py_symbol__eq__ = py_symbol_new("__eq__");
    py_symbol__gt__ = py_symbol_new("__gt__");
    py_symbol__ge__ = py_symbol_new("__ge__");

    py_symbol__len__ = py_symbol_new("__len__");
    py_symbol__str__ = py_symbol_new("__str__");
    py_symbol__getitem__ = py_symbol_new("__getitem__");
    py_symbol__setitem__ = py_symbol_new("__setitem__");
    py_symbol__iter__ = py_symbol_new("__iter__");
    py_symbol__next__ = py_symbol_new("__next__");

    py_symbol_start = py_symbol_new("start");
    py_symbol_stop = py_symbol_new("stop");
    py_symbol_join = py_symbol_new("join");
    py_symbol_run = py_symbol_new("run");
}

py_symbol_t *py_symbol_new(char *value)
{
    int i;
    py_symbol_t *py_symbol;
    vector_each (&py_symbol_vector, i, py_symbol) {
        if (strcmp(py_symbol->value, value) == 0)
            return py_symbol;
    }

    py_symbol_t *this = py_object_alloc(sizeof(py_symbol_t), py_symbol_class);
    this->value = strdup(value);
    vector_push_back(&py_symbol_vector, this);
    return this;
}
