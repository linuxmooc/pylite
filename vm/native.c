#include "root.h"
#include "native.h"
#include "symbol.h"

py_class_t *py_native_class;

void py_native_class_init(void)
{
    py_native_class = py_class_new("native");
}

py_native_t *py_native_new(py_symbol_t *name, handler_t handler) 
{
    py_native_t *this = py_object_alloc(sizeof(py_native_t), py_native_class);

    this->name = name;
    this->handler = handler;
    return this;
}
