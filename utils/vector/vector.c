#include <stdlib.h>
#include <assert.h>
#include "vector.h"

void vector_init(vector_t *this)
{
    this->count = 0;
    this->capacity = 4;
    this->data = malloc(this->capacity * sizeof(void *));
    for (int i = 0; i < this->capacity; i++)
        this->data[i] = NULL;
}

void vector_destroy(vector_t *this)
{
    free(this->data);
}

void vector_push_back(vector_t *this, void *item)
{
    if (this->count >= this->capacity) {
        this->capacity *= 2;
        int size = this->capacity * sizeof(void *);
        this->data = realloc(this->data, size);
    }

    this->data[this->count] = item;
    this->count++;
}

void *vector_pop_back(vector_t *this)
{
    assert(this->count > 0);
    this->count--;
    void *item = this->data[this->count];
    this->data[this->count] = NULL;
    return item;
}

void *vector_get(vector_t *this, int index)
{
    assert(index < this->count);
    return this->data[index];
}

void vector_set(vector_t *this, int index, void *item)
{
    assert(index < this->count);
    this->data[index] = item;
}
