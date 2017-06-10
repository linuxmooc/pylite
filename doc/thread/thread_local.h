root.h:
#define thread_local __thread

error.h:
extern thread_local py_class_t *py_error;

gc.h:
extern thread_local void *c_stack_low;

gc.h:
extern thread_local void *c_stack_high;

vm.h:
extern thread_local py_object_t *vm_stack[VM_STACK_DEPTH];
extern thread_local int sp;
