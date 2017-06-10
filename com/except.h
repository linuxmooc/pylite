#ifndef _COM_EXCEPT_H
#define _COM_EXCEPT_H

#include "compile.h"

extern void compile_try_clause(lambda_t *lambda, ast_t *ast);
extern void compile_raise_clause(lambda_t *lambda, ast_t *ast);

#endif
