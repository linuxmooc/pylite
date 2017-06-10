#include "root.h"
#include "lock.h"
#include "none.h"
#include "error.h"
#include "native.h"
#include "vm.h"

py_class_t *py_lock_class;

py_object_t *py_lock_alloc(py_class_t *py_class)
{
    py_lock_t *this = py_object_alloc(sizeof(py_lock_t), py_class);
    return $(this);
}

py_object_t *py_lock_init(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_lock_t *this = $(argv[0]);
    pthread_mutex_init(&this->mutex, NULL);
    return py_none;
}

py_object_t *py_lock_acquire(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_lock_t *this = $(argv[0]);
    pthread_mutex_lock(&this->mutex);
    return py_none;
}

py_object_t *py_lock_release(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_lock_t *this = $(argv[0]);
    pthread_mutex_unlock(&this->mutex);
    return py_none;
}

native_t py_lock_natives[] = {
    {"__init__", py_lock_init},
    {"acquire", py_lock_acquire},
    {"release", py_lock_release},
    {NULL}
};

void py_lock_class_init()
{
    py_lock_class = py_class_new("Lock");
    py_lock_class->py_alloc = py_lock_alloc;
    py_class_register_natives(py_lock_class, py_lock_natives);
}
