#include <assert.h>
#include <stddef.h>
#include "list.h"

void chain_init(chain_t *this)
{
    this->next = this->prev = NULL;
}

void chain_link(chain_t *left, chain_t *right)
{
    left->next = right;
    right->prev = left;
}

void chain_link3(chain_t *left, chain_t *chain, chain_t *right)
{
    chain_link(left, chain);
    chain_link(chain, right);
}

void chain_unlink(chain_t *this)
{
    chain_link(this->prev, this->next);
    this->next = NULL;
    this->prev = NULL;
}

void chain_prepend(chain_t *this, chain_t *new_chain)
{
    chain_link3(this->prev, new_chain, this);
}

void chain_append(chain_t *this, chain_t *new_chain)
{
    chain_link3(this, new_chain, this->next);
}

void list_init(list_t *this)
{
    this->next = this->prev = this;
}

int list_empty(list_t *this)
{
    return this->next == this;
}

chain_t *list_head(list_t *this)
{
    if (this->next == this)
        return NULL;
    return this->next;
}

chain_t *list_tail(list_t *this)
{
    if (this->next == this)
        return NULL;
    return this->prev;
}

void list_push_head(list_t *this, chain_t *chain)
{
    assert(chain->next == NULL);
    chain_link3(this, chain, this->next); 
}

chain_t *list_pop_head(list_t *this)
{
    if (list_empty(this))
        return NULL;
    chain_t *head = this->next;
    chain_unlink(head);
    return head;
}

void list_push_tail(list_t *this, chain_t *chain)
{
    assert(chain->next == NULL);
    chain_link3(this->prev, chain, this); 
}

chain_t *list_pop_tail(list_t *this)
{
    if (list_empty(this))
        return NULL;
    chain_t *tail = this->prev;
    chain_unlink(tail);
    return tail;
}

void chain_remove(void *chain)
{
    chain_unlink(chain);
}

void *list_front(list_t *this)
{
    return list_head(this);
}

void *list_back(list_t *this)
{
    return list_tail(this);
}

void list_push_front(list_t *this, void *item)
{
    list_push_head(this, item);
}

void list_push_back(list_t *this, void *item)
{
    list_push_tail(this, item);
}

void *list_pop_front(list_t *this)
{
    void *item = list_pop_head(this);
    return item;
}

void *list_pop_back(list_t *this)
{
    void *item = list_pop_tail(this);
    return item;
}
