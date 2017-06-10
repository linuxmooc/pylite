#include "root.h"
#include "mod_temp.h"

py_module_t *py_temp_module;

void py_temp_module_init(void)
{
    py_temp_module = py_module_new(NULL, "__temp__");
    register_py_module(py_temp_module);
}
