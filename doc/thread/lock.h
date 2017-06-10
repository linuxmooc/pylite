#ifndef _PY_LOCK_H
#define _PY_LOCK_H

#include "object.h"
#include "class.h"
#include <pthread.h>

typedef struct py_lock {
    py_object_member_list;
    pthread_mutex_t mutex;
} py_lock_t;

extern py_class_t *py_lock_class;
extern void py_lock_class_init();

#endif
