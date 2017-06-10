#ifndef _UTILS_STACK_H
#define _UTILS_STACK_H

#include <stdbool.h>
#include "vector.h"
typedef vector_t stack_t;

#define stack_init vector_init
#define stack_destroy vector_destroy
#define stack_push vector_push_back
#define stack_pop vector_pop_back

extern bool stack_is_empty(stack_t *this);
extern void *stack_top(stack_t *this);

#endif
