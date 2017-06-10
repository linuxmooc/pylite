#include "root.h"
#include "bool.h"
#include "cast.h"
#include "gc.h"

py_object_t *py_true;
py_object_t *py_false;
py_class_t *py_bool_class;

void py_bool_init(void)
{
    py_true = py_object_new(py_bool_class);
    py_false = py_object_new(py_bool_class);
    gc_add_root(py_true);
    gc_add_root(py_false);
}

py_object_t *py_bool_str(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_object_t *this = argv[0];

    char *text;
    if (this == py_true)
        text = "True";
    else if (this == py_false)
        text = "False";
    else
        text = NULL;
    py_string_t *py_string = py_string_new(text);

    return $(py_string);
}

py_object_t *py_bool_eq(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_object_t *this = $(argv[0]);
    py_object_t *that = $(argv[1]);

    if (this == that)
        return py_true;
    else
        return py_false;
}

py_object_t *py_bool_ne(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_object_t *this = $(argv[0]);
    py_object_t *that = $(argv[1]);

    if (this != that)
        return py_true;
    else
        return py_false;
}

native_t py_bool_natives[] = {
    {"__eq__", py_bool_eq},
    {"__ne__", py_bool_ne},
    {"__str__", py_bool_str},
    {NULL}
};

void py_bool_class_init(void)
{
    py_bool_class = py_class_new("bool");
    py_class_register_natives(py_bool_class, py_bool_natives);
}
