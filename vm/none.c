#include "root.h"
#include "none.h"
#include "cast.h"
#include "gc.h"

py_object_t *py_none;
py_class_t *py_none_class;

void py_none_init()
{
    py_none = py_object_new(py_none_class);
    gc_add_root(py_none);
}

py_object_t *py_none_str(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_object_t *this = $(argv[0]);
    py_string_t *py_string = py_string_new("None");
    return $(py_string);
}

py_object_t *py_none_eq(int argc, py_object_t *argv[])
{
    assert(argc == 2);
    py_object_t *this = $(argv[0]);
    py_object_t *that = $(argv[1]);

    if (this == that)
        return py_true;
    else
        return py_false;
}

py_object_t *py_none_ne(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_object_t *this = $(argv[0]);
    py_object_t *that = $(argv[1]);

    if (this != that)
        return py_true;
    else
        return py_false;
}

native_t py_none_natives[] = {
    {"__eq__", py_none_eq},
    {"__ne__", py_none_ne},
    {"__str__", py_none_str},
    {NULL}
};

void py_none_class_init()
{
    py_none_class = py_class_new("none");
    py_class_register_natives(py_none_class, py_none_natives);
}
