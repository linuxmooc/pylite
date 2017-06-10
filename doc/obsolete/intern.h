#ifndef _VM_INTERN_H
#define _VM_INTERN_H

typedef struct string {
    chain_t chain;
    char *data;
} string_t;
#define to_string(chain) ((string_t *)(chain))

extern void intern_init(void);
extern char *intern(char *string);
extern bool is_interned(char *string);

#endif
