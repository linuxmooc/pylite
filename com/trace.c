#include "root.h"
#include "trace.h"

vector_template(ast_t *, ast_stack);

void ast_stack_init()
{
    stack_init(&ast_stack);
}

void ast_stack_push(ast_t *ast)
{
    stack_push(&ast_stack, ast);
}

void ast_stack_pop()
{
    stack_pop(&ast_stack);
}

ast_t *ast_stack_top()
{
    return stack_top(&ast_stack);
}
