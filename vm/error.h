#ifndef _VM_ERROR_H
#define _VM_ERROR_H

#include "object.h"
#include "class.h"

typedef struct {
    char *file_name;
    char *lambda_name;
    int line_number;
} location_t;

typedef struct {
    py_object_member_list;
    stack_template(location_t, location_stack);
} py_error_t;

extern py_error_t *py_error_new(py_class_t *py_class);
extern void py_error_add(py_error_t *this, char *file_name, 
                         char *lambda_name, int line_number);
extern void py_error_dump(py_error_t *this);

extern py_error_t *py_error;
extern py_class_t *py_exception;
extern py_class_t *py_type_error;
extern py_class_t *py_index_error;
extern py_class_t *py_key_error;
extern py_class_t *py_attr_error;
extern py_class_t *py_name_error;
extern py_class_t *py_zero_division_error;
extern py_class_t *py_stop_iteration;
extern py_class_t *py_runtime_error;
extern py_class_t *py_io_error;
extern void py_errors_init(void);

static inline bool py_error_is_stop()
{
    if ($(py_error) == py_stop_iteration)
        return true; 
    return py_error->py_class == py_stop_iteration;
}

#define vm_throw(py_class)                  \
    do {                                    \
        py_error = py_error_new(py_class);  \
        return NULL;                        \
    } while (0)

#define vm_rethrow()                        \
    do {                                    \
        assert(py_error != NULL);           \
        return NULL;                        \
    } while (0)

#endif
