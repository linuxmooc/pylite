#include "root.h"
#include "double.h"
#include "vm.h"
#include "native.h"
#include "string.h"
#include "gc.h"

py_class_t *py_double_class;

#define PY_INT_MIN -128
#define PY_INT_MAX 128
py_double_t *py_double_pool[PY_INT_MAX - PY_INT_MIN + 1];

static inline py_double_t *py_double_create(double value)
{
    py_double_t *this = py_object_alloc(sizeof(py_double_t), py_double_class);
    this->value = value;
    return this;
}

py_double_t *py_double_new(double value)
{
    long lvalue = (long)value;
    if (value - lvalue == 0 && PY_INT_MIN <= lvalue && lvalue <= PY_INT_MAX)
        return py_double_pool[lvalue - PY_INT_MIN];
    return py_double_create(value);
}

py_object_t *py_double_str(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_double_t *this = $(argv[0]);

    char buf[32];
    double dvalue = this->value;
    long lvalue = (long)this->value;
    if (dvalue - lvalue == 0)
        sprintf(buf, "%ld", lvalue);
    else
        sprintf(buf, "%g", dvalue);
    py_string_t *py_string = py_string_new(buf);
    return $(py_string);
}

native_t py_double_natives[] = {
    {"__str__", py_double_str, 1},
    {NULL}
};

void py_double_class_init()
{
    py_double_class = py_class_new("double");
    py_class_register_natives(py_double_class, py_double_natives);

    for (int i = PY_INT_MIN; i <= PY_INT_MAX; i++) {
        py_double_t *py_double = py_double_create(i);
        gc_add_root($(py_double));
        py_double_pool[i - PY_INT_MIN] = py_double;
    }
}
