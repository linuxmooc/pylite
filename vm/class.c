#include "root.h"
#include "class.h"
#include "native.h"
#include "gc.h"

py_class_t *py_class_new(char *name)
{
    py_class_t *this = py_object_alloc(sizeof(py_class_t), NULL);
    gc_add_root($(this));

    this->name = strdup(name);
    this->py_alloc = NULL;
    this->gc_mark = NULL;
    this->gc_free = NULL;
    this->parent = NULL;
    return this;
}

bool py_class_is_subclass(py_class_t *this, py_class_t *parent)
{
    py_class_t *py_class = this;
    while (py_class != NULL) {
        if (py_class == parent)
            return true;
        py_class = py_class->parent;
    }
    return false;
}

py_object_t *py_class_load_field(py_class_t *this, py_symbol_t *name)
{
    py_object_t *result = py_object_get_field($(this), name);
    if (result)
        return result;

    if (this->parent == NULL)
        return NULL;
    return py_class_load_field(this->parent, name);
}

void py_class_register_natives(py_class_t *this, native_t *natives)
{
    for (native_t *native = natives; native->name; native++) {
        py_symbol_t *name = py_symbol_new(native->name); 
        void *handler = native->handler;

        py_native_t *py_native = py_native_new(name, handler);
        py_object_set_field($(this), name, $(py_native));
    }
}
