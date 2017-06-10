#include "root.h"
#include "intern.h"

char *symbol__init__;
char *symbol__add__;
char *symbol__sub__;

#define HASH_SIZE 256
#define HASH_MASK (HASH_SIZE - 1)
static list_t string_table[HASH_SIZE];

void intern_init(void)
{
    for (int i = 0; i < HASH_SIZE; i++)
        list_init(&string_table[i]);

    symbol__init__ = intern("__init__");
    symbol__add__ = intern("__add__");
    symbol__sub__ = intern("__sub__");
}

unsigned int string_hash_code(char *string)
{
    unsigned int sum = 0;
    while (*string) {
        sum ^= *string;
        string++;
    }
    return sum;
}

static list_t *get_string_list(char *string)
{
    unsigned int index = string_hash_code(string) & HASH_MASK;
    list_t *string_list = string_table + index; 
    return string_list;
}

string_t *string_new(char *data)
{
    string_t *this = malloc(sizeof(string_t));
    chain_init(&this->chain);
    this->data = strdup(data);
    return this;
}

bool is_interned(char *target)
{
    chain_t *chain;
    string_t *string;
    list_t *string_list = get_string_list(target);

    list_each (string_list, chain) {
        string = to_string(chain); 
        if (strcmp(string->data, target) == 0)
            return true;
    }
    return false;
}

char *intern(char *target)
{
    chain_t *chain;
    string_t *string;
    list_t *string_list = get_string_list(target);

    list_each (string_list, chain) {
        string = to_string(chain); 
        if (strcmp(string->data, target) == 0)
            return string->data;
    }

    string = string_new(target);
    list_push_back(string_list, string);
    return string->data;
}
