#ifndef _UTILS_VECTOR_H
#define _UTILS_VECTOR_H

typedef struct vector {
    int capacity;
    int count;
    void **data;
} vector_t;

#define vector_each(this, i, item)  \
    for (i = 0; (i < (this).count) ? (item = (this).data[i], 1) : 0; i++)

extern void vector_init(vector_t *this);
extern void vector_destroy(vector_t *this);
extern void vector_push_back(vector_t *this, void *item);
extern void *vector_pop_back(vector_t *this);
extern void *vector_get(vector_t *this, int index);
extern void vector_set(vector_t *this, int index, void *item);

#endif
