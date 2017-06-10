#include "root.h"
#include "condition.h"
#include "none.h"
#include "error.h"
#include "native.h"
#include "vm.h"

py_class_t *py_cond_class;

py_object_t *py_cond_alloc(py_class_t *py_class)
{
    py_cond_t *this = py_object_alloc(sizeof(py_cond_t), py_class);
    return $(this);
}

py_object_t *py_cond_init(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_cond_t *this = $(argv[0]);
    pthread_mutex_init(&this->mutex, NULL);
    pthread_cond_init(&this->cond, NULL);
    return py_none;
}

py_object_t *py_cond_acquire(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_cond_t *this = $(argv[0]);
    pthread_mutex_lock(&this->mutex);
    return py_none;
}

py_object_t *py_cond_release(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_cond_t *this = $(argv[0]);
    pthread_mutex_unlock(&this->mutex);
    return py_none;
}

py_object_t *py_cond_wait(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_cond_t *this = $(argv[0]);
    pthread_cond_wait(&this->cond, &this->mutex);
    return py_none;
}

py_object_t *py_cond_notify(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_cond_t *this = $(argv[0]);
    pthread_cond_signal(&this->cond);
    return py_none;
}

native_t py_cond_natives[] = {
    {"__init__", py_cond_init},
    {"acquire", py_cond_acquire},
    {"release", py_cond_release},
    {"wait", py_cond_wait},
    {"notify", py_cond_notify},
    {NULL}
};

void py_cond_class_init()
{
    py_cond_class = py_class_new("Lock");
    py_cond_class->py_alloc = py_cond_alloc;
    py_class_register_natives(py_cond_class, py_cond_natives);
}
