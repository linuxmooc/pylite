#include "root.h"
#include "mod_math.h"

py_module_t *py_math_module;

py_object_t *py_math_sin(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    double angle = cast_double(argv[0]);
    double result = sin(angle);
    py_double_t *py_result = py_double_new(result);
    return $(py_result);
}

py_object_t *py_math_cos(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    double angle = cast_double(argv[0]);
    double result = cos(angle);
    py_double_t *py_result = py_double_new(result);
    return $(py_result);
}

py_object_t *py_math_pow(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    double x = cast_double(argv[0]);
    double y = cast_double(argv[1]);
    double result = pow(x, y);
    py_double_t *py_result = py_double_new(result);
    return $(py_result);
}

void py_math_module_init(void)
{
    py_math_module = py_module_new(NULL, "math");
    register_py_module(py_math_module);

    py_module_import_double(py_math_module, "pi", 3.14159265359);
    py_module_import_lambda(py_math_module, "sin", py_math_sin);
    py_module_import_lambda(py_math_module, "cos", py_math_cos);
    py_module_import_lambda(py_math_module, "pow", py_math_pow);
}
