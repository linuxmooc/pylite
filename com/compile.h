#ifndef _COM_COMPILE_H
#define _COM_COMPILE_H

#include "operand.h"
#include "lambda.h"
#include "scope.h"
#include "ast.h"
#include "lex.h"
#include "gram.tab.h"

extern void compile_error(ast_t *ast, char *format, ...);
extern lambda_t *compile_module(ast_t *ast);
extern void compile_clause(lambda_t *lambda, ast_t *ast);
extern void store_operand(lambda_t *lambda, operand_t *target);
extern void compile_param_list(lambda_t *def, ast_t *param_list);

#define emit_insn(...) lambda_emit_insn(lambda, __VA_ARGS__)
#define emit_label(label) lambda_emit_label(lambda, label)
#define new_label() lambda_new_label(lambda)

#endif
