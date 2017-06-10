#include "root.h"
#include "thread.h"
#include "lock.h"
#include "condition.h"
#include "mod_threading.h"

py_module_t *py_threading_module;

#define import(type, ...) \
    py_module_import_##type(py_threading_module, __VA_ARGS__)
void py_threading_module_init()
{
    py_threading_module = py_module_new(NULL, "threading");
    register_py_module(py_threading_module);
    import(class, "Thread", py_thread_class);
    import(class, "Lock", py_lock_class);
    import(class, "Condition", py_cond_class);
}
