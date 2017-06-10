#ifndef _COM_ASSIGN_H
#define _COM_ASSIGN_H

#include "compile.h"

extern void store_operand(lambda_t *lambda, operand_t *target);
extern void unpack_tuple(lambda_t *lambda, ast_t *left);
extern void define_tuple_items(ast_t *tuple);
extern void compile_assign_clause(lambda_t *lambda, ast_t *ast);

#endif
