#ifndef _COM_LOOP_H
#define _COM_LOOP_H

#include "compile.h"

extern void compile_break_clause(lambda_t *lambda, ast_t *ast);
extern void compile_continue_clause(lambda_t *lambda, ast_t *ast);
extern void compile_while_clause(lambda_t *lambda, ast_t *ast);
extern void compile_for_clause(lambda_t *lambda, ast_t *ast);

#endif
