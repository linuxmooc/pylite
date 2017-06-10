#include "root.h"
#include "string.h"
#include "cast.h"
#include "gc.h"

py_class_t *py_double_class;

py_object_t *py_double_str(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
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
    {"__str__", py_double_str},
    {NULL}
};

void py_double_class_init()
{
    py_double_class = py_class_new("double");
    py_class_register_natives(py_double_class, py_double_natives);
}
