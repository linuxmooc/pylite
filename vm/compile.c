#include <libgen.h>
#include "root.h"
#include "compile.h"

static bool succeed(int status)
{
    return (status != -1) && WIFEXITED(status) && (WEXITSTATUS(status) == 0);
}

void check_compiler(void)
{
    int status = system("which pycom >/dev/null");
    if (!succeed(status)) {
        fprintf(stderr, "The compiler 'pycom' is not found in PATH environment!\n");
        exit(EXIT_FAILURE);
    }
}

void compile_python(char *py_path, char *xml_path)
{
    char command[256];

    sprintf(command, "pycom %s -o %s", py_path, xml_path);
    int status = system(command);
    if (!succeed(status)) {
        fprintf(stderr, "Execute compiler('%s') failed!\n", command);
        exit(EXIT_FAILURE);
    }
}
