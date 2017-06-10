#ifndef _UTILS_STACK_H
#define _UTILS_STACK_H

#include <stdbool.h>
#include "vector.h"

#define stack_template vector_template
#define stack_init vector_init
#define stack_destroy vector_destroy
#define stack_push vector_push_back
#define stack_pop vector_pop_back
#define stack_is_empty(this) ((this)->count == 0)
#define stack_top(this)                                     \
    ({                                                      \
        assert(!stack_is_empty(this));                      \
        (this)->data[(this)->count - 1];                    \
    })

#endif
