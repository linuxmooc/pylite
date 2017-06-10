#include "root.h"
#include "module.h"
#include "compile.h"
#include "objects.h"
#include "error.h"
#include "mod.h"
#include "gc.h"
#include "alloc.h"

static char *usage_text =
"pyvm, version 0.1\n"
"  pyvm file.py\n"
"  pyvm file.xml\n"
;

static void usage(void)
{
    printf("%s", usage_text);
    exit(EXIT_FAILURE);
}

int program_argc;
char **program_argv;

int main(int argc, char *argv[])
{
    program_argc = argc;
    program_argv = argv;
    if (argc == 1)
        usage();
    char *input_path = argv[1];

    check_compiler();
    xml_stdout = xml_file_new("/dev/stdout");

    void *dummy;
    c_stack_high = &dummy;
    gc_setup(1024 * 1024);

    py_symbol_vector_init();
    py_module_vector_init();

    py_classes_init();
    py_bool_init();
    py_none_init();
    py_errors_init();
    py_modules_init();
    open_main_module(input_path);

    xml_file_delete(xml_stdout);
    return 0;
}
