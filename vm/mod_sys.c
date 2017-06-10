#include "root.h"
#include "mod_sys.h"
#include "list.h"
#include "file.h"

py_module_t *py_sys_module;

py_object_t *py_sys_exit(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_object_t *py_status = argv[0]; 
    int status = py_object_to_integer(py_status);
    
    exit(status);
    return py_none;
}

extern int program_argc;
extern char **program_argv;

py_object_t *build_argv()
{
    py_list_t *py_argv = py_list_new(0);
    for (int i = 1; i < program_argc; i++) {
        void *py_arg = py_string_new(program_argv[i]);
        py_list_push_back(py_argv, py_arg);
    }
    return $(py_argv);
}

#define import(type, ...) \
    py_module_import_##type(py_sys_module, __VA_ARGS__)
void py_sys_module_init(void)
{
    py_sys_module = py_module_new(NULL, "sys");
    register_py_module(py_sys_module);

    import(lambda, "exit", py_sys_exit);
    import(object, "argv", build_argv());

    py_file_t *py_stdin = py_file_fopen("/dev/stdin", "r"); 
    py_file_t *py_stdout = py_file_fopen("/dev/stdout", "w"); 
    py_file_t *py_stderr = py_file_fopen("/dev/stderr", "w"); 

    import(object, "stdin", $(py_stdin));
    import(object, "stdout", $(py_stdout));
    import(object, "stderr", $(py_stderr));
}
