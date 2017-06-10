#include "root.h"
#include "thread.h"
#include "none.h"
#include "error.h"
#include "native.h"
#include "vm.h"

py_class_t *py_thread_class;

py_object_t *py_thread_alloc(py_class_t *py_class)
{
    py_thread_t *this = py_object_alloc(sizeof(py_thread_t), py_class);
    this->pthread = 0;
    return $(this);
}

py_object_t *py_thread_init(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_thread_t *this = $(argv[0]);
    return py_none;
}

void *py_thread_entry(void *arg)
{
    py_thread_t *this = arg;
    py_object_t *py_method = py_object_load_field($(this), py_symbol_run);
    assert(py_method);
    py_object_t *py_result = vm_call(py_method, 0, vm_stack + sp);
    assert(py_result);
    return NULL;
}

py_object_t *py_thread_start(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_thread_t *this = $(argv[0]);

    pthread_t pthread;
    pthread_create(&pthread, NULL, py_thread_entry, this);
    this->pthread = pthread;

    return py_none;
}

py_object_t *py_thread_join(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_thread_t *this = $(argv[0]);

    int error = pthread_join(this->pthread, NULL);
    if (error != 0)
        vm_throw(py_runtime_error);

    return py_none;
}

native_t py_thread_natives[] = {
    {"__init__", py_thread_init},
    {"start", py_thread_start},
    {"join", py_thread_join},
    {NULL}
};

void py_thread_class_init()
{
    py_thread_class = py_class_new("Thread");
    py_thread_class->py_alloc = py_thread_alloc;
    py_class_register_natives(py_thread_class, py_thread_natives);
}
