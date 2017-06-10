#include "root.h"
#include "express.h"
#include "assign.h"

void store_operand(lambda_t *lambda, operand_t *target)
{
    switch (target->type) {
        case OPERAND_GLOBAL:
            emit_insn(OP_STORE_GLOBAL, "%S", target->svalue);
            break;

        case OPERAND_LOCAL:
            emit_insn(OP_STORE_LOCAL, "%i", target->ivalue);
            break;

        case OPERAND_ITEM:
            emit_insn(OP_STORE_ITEM, NULL);
            break;

        case OPERAND_FIELD:
            emit_insn(OP_STORE_FIELD, "%S", target->svalue);
            break;

        default:
            assert(false);
    }
}

#define map(a, b) case a: return b
int translate_assign_operator(ast_t *operator)
{
    assert(operator->type == AST_TOKEN);
    switch (operator->token) {
        map(TOKEN_MUL_ASSIGN, OP_MUL);
        map(TOKEN_DIV_ASSIGN, OP_DIV);
        map(TOKEN_MOD_ASSIGN, OP_MOD);
        map(TOKEN_ADD_ASSIGN, OP_ADD);
        map(TOKEN_SUB_ASSIGN, OP_SUB);
        map(TOKEN_LSH_ASSIGN, OP_LSH);
        map(TOKEN_RSH_ASSIGN, OP_RSH);
        map(TOKEN_AND_ASSIGN, OP_AND);
        map(TOKEN_XOR_ASSIGN, OP_XOR);
        map(TOKEN_OR_ASSIGN, OP_OR);
    }
    assert(false);
    return 0;
}
#undef map

void unpack_tuple(lambda_t *lambda, ast_t *left)
{
    int i;
    ast_t *id;

    ast_each_child (left, i, id) {
        emit_insn(OP_LOAD_CONST, "%d", i);
        emit_insn(OP_DUP_TOP, "%i", 1);
        emit_insn(OP_LOAD_ITEM, NULL);
        operand_t *target = compile_express(lambda, id);
        store_operand(lambda, target);
    }
}

void define_tuple_items(ast_t *tuple)
{
    int i;
    ast_t *id;
    ast_each_child (tuple, i, id) {
        if (id->type == AST_TOKEN) {
            char *name = id->svalue;
            define_symbol(name);
        }
    }
}

void compile_assign_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *left = ast_get_child(ast, 0);
    ast_t *operator = ast_get_child(ast, 1);
    ast_t *right = ast_get_child(ast, 2);

    if (left->type == AST_TOKEN) {
        char *name = left->svalue;
        define_symbol(name);
    } 
    
    if (left->type == AST_TUPLE_EXPRESS)
        define_tuple_items(left);

    if (operator->token == '=') {
        evaluate_express(lambda, right);
    } else {
        int opcode = translate_assign_operator(operator);
        build_binary_express(lambda, left, opcode, right);
    }

    if (left->type == AST_TUPLE_EXPRESS) {
        unpack_tuple(lambda, left);
        emit_insn(OP_POP, NULL);
    } else {
        operand_t *target = compile_express(lambda, left);
        store_operand(lambda, target);
    }
}
