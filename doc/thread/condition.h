#ifndef _PY_CONDITION_H
#define _PY_CONDITION_H

#include "object.h"
#include "class.h"
#include <pthread.h>

typedef struct py_cond {
    py_object_member_list;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} py_cond_t;

extern py_class_t *py_cond_class;
extern void py_cond_class_init();

#endif
