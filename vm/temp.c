#include "root.h"
#include "temp.h"

py_class_t *py_temp_class;

py_temp_t *py_temp_new()
{
    py_temp_t *this = py_object_alloc(sizeof(py_temp_t), py_temp_class);
    return this;
}

void py_temp_class_init(void)
{
    py_temp_class = py_class_new("temp");
}


