#include "root.h"
#include "branch.h"
#include "express.h"

bool is_and_express(ast_t *ast)
{
    if (ast->type != AST_BINARY_EXPRESS)
        return false;
    ast_t *operator = ast_get_child(ast, 1);
    return operator->token == TOKEN_AND;
}

bool is_or_express(ast_t *ast)
{
    if (ast->type != AST_BINARY_EXPRESS)
        return false;
    ast_t *operator = ast_get_child(ast, 1);
    return operator->token == TOKEN_OR;
}

bool is_compare_express(ast_t *ast)
{
    if (ast->type != AST_BINARY_EXPRESS)
        return false;
    ast_t *operator = ast_get_child(ast, 1);
    int token = operator->token;
    return token == TOKEN_EQ || token == TOKEN_NE ||
           token == '<' || token == TOKEN_LE ||
           token == '>' || token == TOKEN_GE;
}

bool is_not_express(ast_t *ast)
{
    if (ast->type != AST_UNARY_EXPRESS)
        return false;
    ast_t *operator = ast_get_child(ast, 0);
    return operator->token == '!';
}

#define map(a, b) case a: return b
int translate_compare_true(int operator)
{
    switch (operator) {
        map(TOKEN_EQ, OP_JEQ);
        map(TOKEN_NE, OP_JNE);
        map('<', OP_JLT);
        map(TOKEN_LE, OP_JLE);
        map('>', OP_JGT);
        map(TOKEN_GE, OP_JGE);
    }
    assert(0);
    return 0;
}

int translate_compare_false(int operator)
{
    switch (operator) {
        map(TOKEN_EQ, OP_JNE);
        map(TOKEN_NE, OP_JEQ);
        map('<', OP_JGE);
        map(TOKEN_LE, OP_JGT);
        map('>', OP_JLE);
        map(TOKEN_GE, OP_JLT);
    }
    assert(0);
    return 0;
}
#undef map

void branch_compare_express(lambda_t *lambda, ast_t *ast, 
                            bool true_label, operand_t *label)
{
    ast_t *left = ast_get_child(ast, 0);
    ast_t *operator = ast_get_child(ast, 1);
    assert(operator->type == AST_TOKEN);
    ast_t *right = ast_get_child(ast, 2);

    evaluate_express(lambda, right);
    evaluate_express(lambda, left);
    int opcode;
    if (true_label)
        opcode = translate_compare_true(operator->token);
    else
        opcode = translate_compare_false(operator->token);
    emit_insn(opcode, "%o", label);
}

#define left ast_get_child(ast, 0)
#define right ast_get_child(ast, 2)

void branch_true(lambda_t *lambda, ast_t *ast, operand_t *true_label)
{
    if (is_and_express(ast)) {
        operand_t *false_label = new_label();
        branch_false(lambda, left, false_label);
        branch_true(lambda, right, true_label);
        emit_label(false_label);
        return; 
    } 

    if (is_or_express(ast)) {
        branch_true(lambda, left, true_label);
        branch_true(lambda, right, true_label);
        return;
    }

    if (is_compare_express(ast)) {
        branch_compare_express(lambda, ast, true, true_label);
        return;
    }

    if (is_not_express(ast)) {
        ast_t *express = ast_get_child(ast, 1);
        branch_false(lambda, express, true_label);
        return;
    }

    operand_t *source = evaluate_express(lambda, ast);
    emit_insn(OP_JTRUE, "%o", true_label);
}

void branch_false(lambda_t *lambda, ast_t *ast, operand_t *false_label)
{
    if (is_and_express(ast)) {
        branch_false(lambda, left, false_label);
        branch_false(lambda, right, false_label);
        return; 
    } 

    if (is_or_express(ast)) {
        operand_t *true_label = new_label();
        branch_true(lambda, left, true_label);
        branch_false(lambda, right, false_label);
        emit_label(true_label);
        return;
    }

    if (is_compare_express(ast)) {
        branch_compare_express(lambda, ast, false, false_label);
        return;
    }

    if (is_not_express(ast)) {
        ast_t *express = ast_get_child(ast, 1);
        branch_true(lambda, express, false_label);
        return;
    }

    operand_t *source = evaluate_express(lambda, ast);
    emit_insn(OP_JFALSE, "%o", false_label);
}

#undef left
#undef right
