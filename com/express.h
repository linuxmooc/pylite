#ifndef _COM_EXPRESS_H
#define _COM_EXPRESS_H

#include "compile.h"
extern operand_t *load_operand(lambda_t *lambda, operand_t *source);
extern operand_t *compile_express(lambda_t *lambda, ast_t *ast);
extern operand_t *evaluate_express(lambda_t *lambda, ast_t *ast);
extern operand_t *build_binary_express(lambda_t *lambda, ast_t *left, int opcode, ast_t *right);

#endif
