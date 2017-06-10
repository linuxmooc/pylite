#include "root.h"
#include "mod_os.h"
#include <unistd.h>

py_module_t *py_os_module;

py_object_t *py_os_fork(int argc, py_object_t *argv[])
{
    assert_argc(argc, 0);
    pid_t pid = fork();
    py_double_t *py_result = py_double_new(pid);
    return $(py_result);
}

py_object_t *py_os_dup(int argc, py_object_t *argv[])
{
    assert_argc(argc, 0);
    return py_none; 
}

py_object_t *py_os_dup2(int argc, py_object_t *argv[])
{
    assert_argc(argc, 0);
    return py_none; 
}

py_object_t *py_os_access(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_object_t *py_path = argv[0];
    py_object_t *py_mode = argv[1];

    if (py_path->py_class != py_string_class)
        vm_throw(py_type_error);
    char *path = py_object_to_chars(py_path);

    if (py_mode->py_class != py_double_class)
        vm_throw(py_type_error);
    int mode = py_object_to_integer(py_mode);

    int error = access(path, mode);
    if (error == 0)
        return py_true; 
    else
        return py_false;
}

#define import(type, ...) \
    py_module_import_##type(py_os_module, __VA_ARGS__)
void py_os_module_init(void)
{
    py_os_module = py_module_new(NULL, "os");
    register_py_module(py_os_module);

    import(lambda, "fork", py_os_fork);
    import(lambda, "dup", py_os_dup);
    import(lambda, "dup2", py_os_dup2);
    import(lambda, "access", py_os_access);
    import(double, "R_OK", R_OK);
    import(double, "W_OK", W_OK);
    import(double, "X_OK", X_OK);
}
