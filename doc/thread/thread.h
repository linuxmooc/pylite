#ifndef _PY_THREAD_H
#define _PY_THREAD_H

#include "object.h"
#include "class.h"
#include <pthread.h>

typedef struct py_thread {
    py_object_member_list;
    pthread_t pthread;
} py_thread_t;

extern py_class_t *py_thread_class;
extern void py_thread_class_init();

#endif
