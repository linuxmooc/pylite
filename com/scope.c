#include "root.h"
#include "symbol.h"
#include "scope.h"

stack_template(scope_t *, scope_stack);

scope_t *scope_new(int type)
{
    scope_t *this = malloc(sizeof(scope_t));
    this->type = type;
    this->lambda = NULL;
    this->class_name = NULL;
    list_init(&this->list);
    return this;
}

void scope_delete(scope_t *this)
{
    if (this->class_name)
        free(this->class_name);
    free(this);
}

void scope_dump(scope_t *this, xml_file_t *xml_file)
{
    chain_t *chain;
    list_each (&this->list, chain) {
        symbol_t *symbol = to_symbol(chain);
        symbol_dump(symbol, xml_file);
    }
}

void scope_insert_symbol(scope_t *this, symbol_t *symbol)
{
    if (scope_lookup_symbol(this, symbol->name))
        return;
    list_push_back(&this->list, symbol);
}

symbol_t *scope_lookup_symbol(scope_t *this, char *name)
{
    chain_t *chain;
    list_each (&this->list, chain) {
        symbol_t *symbol = to_symbol(chain);
        if (strcmp(symbol->name, name) == 0)
            return symbol;
    }
    return NULL;
}

symbol_t *lookup_symbol(char *name)
{
    scope_t *top = scope_stack_top();
    symbol_t *symbol = scope_lookup_symbol(top, name);
    return symbol;
}

symbol_t *define_symbol(char *name)
{
    symbol_t *symbol = lookup_symbol(name);
    if (symbol != NULL)
        return symbol;

    scope_t *top = scope_stack_top();
    switch (top->type) {
        case SCOPE_GLOBAL:
            return insert_global_symbol(name);

        case SCOPE_LOCAL:
            return insert_local_symbol(name);

        case SCOPE_CLASS:
            return insert_class_symbol(name);

        default:
            return NULL;
    }
}

symbol_t *insert_global_symbol(char *name)
{
    scope_t *top = scope_stack_top();
    symbol_t *symbol = symbol_new(SCOPE_GLOBAL, name);
    scope_insert_symbol(top, symbol);
    return symbol;
}

symbol_t *insert_local_symbol(char *name)
{
    scope_t *top = scope_stack_top();
    symbol_t *symbol = symbol_new(SCOPE_LOCAL, name);
    scope_insert_symbol(top, symbol);

    lambda_t *lambda = top->lambda;
    lambda->local_count++;
    symbol->offset = 0 - lambda->local_count;
    return symbol;
}

symbol_t *insert_param_symbol(char *name)
{
    scope_t *top = scope_stack_top();
    symbol_t *symbol = symbol_new(SCOPE_LOCAL, name);
    scope_insert_symbol(top, symbol);

    lambda_t *lambda = top->lambda;
    symbol->offset = lambda->param_count;
    lambda->param_count++;
    return symbol;
}

symbol_t *insert_class_symbol(char *name)
{
    scope_t *top = scope_stack_top();
    symbol_t *symbol = symbol_new(SCOPE_CLASS, name);
    scope_insert_symbol(top, symbol);
    return symbol;
}

void scope_stack_init(void)
{
    stack_init(&scope_stack);
}

scope_t *scope_stack_top(void)
{
    return stack_top(&scope_stack);
}

void scope_stack_push(scope_t *scope)
{
    stack_push(&scope_stack, scope);
}

void scope_stack_pop()
{
    scope_t *scope = stack_pop(&scope_stack);
    scope_delete(scope);
}

void enter_global_scope()
{
    scope_t *scope = scope_new(SCOPE_GLOBAL);
    scope_stack_push(scope);
}

void leave_global_scope()
{
    scope_stack_pop();
}

void enter_class_scope(char *class_name)
{
    scope_t *scope = scope_new(SCOPE_CLASS);
    scope->class_name = strdup(class_name);
    scope_stack_push(scope);
}

void leave_class_scope()
{
    scope_stack_pop();
}

void enter_local_scope(lambda_t *lambda)
{
    scope_t *scope = scope_new(SCOPE_LOCAL);
    scope->lambda = lambda;
    scope_stack_push(scope);
}

void leave_local_scope()
{
    scope_stack_pop();
}
