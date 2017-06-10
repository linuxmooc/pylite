#ifndef _VM_ASM_H
#define _VM_ASM_H

#include "lambda.h"
extern int py_lambda_build_operand(py_lambda_t *this, 
                                   xml_token_t *xml_operand);
extern void py_lambda_assemble(py_lambda_t *this, xml_tree_t *xml_root);

#endif
