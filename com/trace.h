#ifndef _COM_TRACE_H
#define _COM_TRACE_H

#include "ast.h"

extern void ast_stack_init();
extern void ast_stack_push(ast_t *ast);
extern void ast_stack_pop();
extern ast_t *ast_stack_top();

#endif
