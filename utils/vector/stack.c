#include <assert.h>
#include "stack.h"

bool stack_is_empty(stack_t *this)
{
    return this->count == 0;
}

void *stack_top(stack_t *this)
{
    assert(!stack_is_empty(this));
    return this->data[this->count - 1];
}
