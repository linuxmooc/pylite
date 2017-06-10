#ifndef _COM_BRANCH_H
#define _COM_BRANCH_H

#include "compile.h"
extern void branch_true(lambda_t *lambda, ast_t *ast, operand_t *true_label);
extern void branch_false(lambda_t *lambda, ast_t *ast, operand_t *false_label);

#endif
