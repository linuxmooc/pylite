#ifndef _UTILS_LIST_H
#define _UTILS_LIST_H

#define container_of(type, member, pointer)                                  \
    ((type*) ((ulong)pointer - offset_of(type, member)))

#define list_each(this, chain)                                               \
    for (chain = (this)->next;                                               \
         chain != (this);                                                    \
         chain = chain->next)

#define list_each_reverse(this, chain)                                       \
    for (chain = (this)->prev;                                               \
         chain != (this);                                                    \
         chain = chain->prev)

typedef struct chain {
    struct chain *next, *prev;
} chain_t;
typedef chain_t list_t;

extern void chain_init(chain_t *this);
extern void chain_link(chain_t *left, chain_t *right);
extern void chain_unlink(chain_t *this);
extern void chain_prepend(chain_t *this, chain_t *new_chain);
extern void chain_append(chain_t *this, chain_t *new_chain);

extern void list_init(list_t *this);
extern int list_empty(list_t *this);

extern chain_t *list_head(list_t *this);
extern chain_t *list_tail(list_t *this);
extern void list_push_head(list_t *this, chain_t *chain);
extern chain_t *list_pop_head(list_t *this);
extern void list_push_tail(list_t *this, chain_t *chain);
extern chain_t *list_pop_tail(list_t *this);

extern void chain_remove(void *item);
extern void *list_front(list_t *this);
extern void *list_back(list_t *this);
extern void list_push_front(list_t *this, void *item);
extern void list_push_back(list_t *this, void *item);
extern void *list_pop_front(list_t *this);
extern void *list_pop_back(list_t *this);

#endif
