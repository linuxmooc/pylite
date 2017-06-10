#include "root.h"
#include "mod_builtin.h"
#include "range.h"
#include "list.h"
#include "dict.h"
#include "file.h"

py_module_t *py_builtin_module;

py_object_t *py_builtin_len(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_object_t *this = argv[0];

    py_object_t *py_method = py_object_load_field(this, py_symbol__len__);
    if (py_method == NULL)
        vm_throw(py_attr_error);

    py_object_t *py_result = vm_call(py_method, 0, argv);
    return py_result;
}

py_object_t *py_builtin_open(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_object_t *py_path = argv[0];
    py_object_t *py_mode = argv[1];

    if (py_path->py_class != py_string_class)
        vm_throw(py_type_error);
    if (py_mode->py_class != py_string_class)
        vm_throw(py_type_error);

    char *path = py_object_to_chars(py_path);
    char *mode = py_object_to_chars(py_mode);
    py_file_t *py_file = py_file_fopen(path, mode);
    return $(py_file);
}

py_object_t *py_builtin_isinstance(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_object_t *instance = argv[0]; 
    py_object_t *prototype = argv[1];

    if (!py_object_is_class(prototype))
        vm_throw(py_type_error);

    if (py_class_is_subclass(instance->py_class, $(prototype)))
        return py_true;
    else
        return py_false;
}

py_object_t *py_builtin_import(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_string_t *py_name = cast_object(argv[0], string);
    py_module_t *py_module = load_py_module(".", py_name->value);
    return $(py_module);
}

py_object_t *py_builtin_int(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_object_t *arg = argv[0];
    py_class_t *py_class = arg->py_class;

    if (py_class == py_double_class)
        return arg;

    if (py_class == py_string_class) {
        py_string_t *py_string = $(arg);
        int number = atoi(py_string->value);
        py_double_t *py_double = py_double_new(number);
        return $(py_double);
    }

    vm_throw(py_type_error);
    return NULL;
}

#define import(type, ...) \
    py_module_import_##type(py_builtin_module, __VA_ARGS__)
void py_builtin_module_init(void)
{
    py_builtin_module = py_module_new(NULL, "__builtin__");
    register_py_module(py_builtin_module);

    import(lambda, "len", py_builtin_len);
    import(lambda, "open", py_builtin_open);
    import(lambda, "isinstance", py_builtin_isinstance);
    import(lambda, "__import__", py_builtin_import);
    import(lambda, "int", py_builtin_int);

    import(class, "object", py_object_class);
    import(class, "list", py_list_class);
    import(class, "dict", py_dict_class);
    import(class, "range", py_range_class);

    import(class, "Exception", py_exception);
    import(class, "TypeError", py_type_error);
    import(class, "IndexError", py_index_error);
    import(class, "KeyError", py_key_error);
    import(class, "AttributeError", py_attr_error);
    import(class, "NameError", py_name_error);
    import(class, "ZeroDivisionError", py_zero_division_error);
    import(class, "StopIteration", py_stop_iteration);
    import(class, "RuntimeError", py_runtime_error);
    import(class, "IOError", py_io_error);
}
