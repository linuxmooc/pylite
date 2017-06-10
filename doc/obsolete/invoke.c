static inline py_object_t *vm_invoke(py_object_t *py_instance, py_object_t *py_name, int argc)
{
    py_object_t *py_method = py_object_load_field(py_instance, py_name);
    if (py_method == NULL)
        vm_throw(py_attr_error);

    py_object_t *py_result = vm_call(py_method, argc);
    return py_result;
}


