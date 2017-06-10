#ifndef _PY_SOCKET_H
#define _PY_SOCKET_H

#include "object.h"
#include "class.h"

typedef struct py_socket {
    py_object_member_list;
    int fd;
    int family;
} py_socket_t;

extern py_class_t *py_socket_class;
extern void py_socket_class_init();

#endif
