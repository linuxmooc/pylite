#ifndef _VM_CAST_H
#define _VM_CAST_H

#include "object.h"
#include "class.h"

#include "double.h"
#include "string.h"
#include "none.h"
#include "bool.h"

#include "error.h"
#include "vm.h"
#include "native.h"

#define cast_double(py_object)                          \
    ({                                                  \
        if (py_object->py_class != py_double_class)     \
            vm_throw(py_type_error);                    \
        py_double_t *py_double = $(py_object);          \
        double value = py_double->value;                \
        value;                                          \
    })

#define cast_integer(py_object)                         \
    ({                                                  \
        if (py_object->py_class != py_double_class)     \
            vm_throw(py_type_error);                    \
        py_double_t *py_double = $(py_object);          \
        int value = (int) py_double->value;             \
        value;                                          \
    })

#define cast_range(py_object, start, end)               \
    ({                                                  \
        if (py_object->py_class != py_double_class)     \
            vm_throw(py_type_error);                    \
        py_double_t *py_double = $(py_object);          \
        int value = (int) py_double->value;             \
        if (!(start <= value && value < end))           \
            vm_throw(py_index_error);                   \
        value;                                          \
    })

#define cast_string(py_object)                          \
    ({                                                  \
        if (py_object->py_class != py_string_class)     \
            vm_throw(py_type_error);                    \
        py_string_t *py_string = $(py_object);          \
        char *value = py_string->value;                 \
        value;                                          \
    })

#define cast_object(py_object, kind)                    \
    ({                                                  \
        if (py_object->py_class != py_##kind##_class)   \
            vm_throw(py_type_error);                    \
        (py_##kind##_t *) py_object;                    \
    })

#define assert_argc(argc, count)                        \
    do {                                                \
        if (argc != count)                              \
            vm_throw(py_type_error);                    \
    } while (0)

#endif
