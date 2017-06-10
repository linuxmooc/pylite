#ifndef _COM_SCOPE_H
#define _COM_SCOPE_H

#include "symbol.h"
#include "lambda.h"

enum {
    SCOPE_GLOBAL,
    SCOPE_LOCAL,
    SCOPE_CLASS
};

typedef struct {
    int type;
    list_t list;
    lambda_t *lambda;
    char *class_name;
} scope_t;

scope_t *scope_new(int type);
extern void scope_delete(scope_t *this);
extern void scope_dump(scope_t *this, xml_file_t *xml_file);
extern void scope_insert_symbol(scope_t *this, symbol_t *symbol);
extern symbol_t *scope_lookup_symbol(scope_t *this, char *name);

extern void enter_global_scope();
extern void leave_global_scope();
extern void enter_class_scope(char *class_name);
extern void leave_class_scope();
extern void enter_local_scope(lambda_t *lambda);
extern void leave_local_scope();

extern void scope_stack_init(void);
extern scope_t *scope_stack_top(void);

extern symbol_t *lookup_symbol(char *name);
extern symbol_t *define_symbol(char *name);
extern symbol_t *insert_global_symbol(char *name);
extern symbol_t *insert_local_symbol(char *name);
extern symbol_t *insert_param_symbol(char *name);
extern symbol_t *insert_class_symbol(char *name);

#endif
