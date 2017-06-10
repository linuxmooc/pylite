#include "root.h"
#include "express.h"
#include "branch.h"
#include "trace.h"

operand_t *build_binary_express(lambda_t *lambda, ast_t *left, int opcode, ast_t *right)
{
    evaluate_express(lambda, right);
    evaluate_express(lambda, left);
    emit_insn(opcode, NULL);
    return operand_stack;
}

/**
 * Translation of bool expression:
 *
 *         branch_true true
 *         push false
 *         jmp exit
 * true:   push true
 * exit:
 */
operand_t *compile_bool_express(lambda_t *lambda, ast_t *ast)
{
    operand_t *true_label = new_label();
    operand_t *exit_label = new_label();

    branch_true(lambda, ast, true_label);
    emit_insn(OP_LOAD_CONST, "%o", operand_false);
    emit_insn(OP_JMP, "%o", exit_label);
    emit_label(true_label);
    emit_insn(OP_LOAD_CONST, "%o", operand_true);
    emit_label(exit_label);
    return operand_stack;
}

operand_t *compile_lambda_express(lambda_t *lambda, ast_t *ast)
{
    ast_t *param_list = ast_get_child(ast, 0);
    ast_t *express = ast_get_child(ast, 1);

    lambda_t *def = lambda_new("__anonymous__");
    enter_local_scope(def);
    compile_param_list(def, param_list);
    evaluate_express(def, express);
    lambda_emit_insn(def, OP_RETURN, NULL);
    leave_local_scope();

    emit_insn(OP_LOAD_CONST, "%o", def);
    return operand_stack;
}

#define map(a, b) case a: opcode = b; break;
operand_t *compile_binary_express(lambda_t *lambda, ast_t *ast)
{
    ast_t *left = ast_get_child(ast, 0);
    ast_t *operator = ast_get_child(ast, 1);
    ast_t *right = ast_get_child(ast, 2);

    int opcode = -1;
    assert(operator->type == AST_TOKEN);
    switch (operator->token) {
        map('|', OP_OR);
        map('&', OP_AND);
        map('^', OP_XOR);
        map(TOKEN_LSH, OP_LSH);
        map(TOKEN_RSH, OP_RSH);
        map('+', OP_ADD);
        map('-', OP_SUB);
        map('*', OP_MUL);
        map('/', OP_DIV);
        map('%', OP_MOD);

        case TOKEN_OR:
        case TOKEN_AND:
        case TOKEN_EQ:
        case TOKEN_NE:
        case '<':
        case TOKEN_LE:
        case '>':
        case TOKEN_GE:
            return compile_bool_express(lambda, ast);

        default:
            assert(false);
    }

    operand_t *target = build_binary_express(lambda, left, opcode, right);
    return target;
}
#undef map

operand_t *compile_unary_express(lambda_t *lambda, ast_t *ast)
{
    ast_t *operator = ast_get_child(ast, 0);
    ast_t *express = ast_get_child(ast, 1);

    assert(operator->type == AST_TOKEN);
    if (ast_is_token(express, TOKEN_DOUBLE_CONST)) {
        int value = express->dvalue;
        switch (operator->token) {
            case '+':
                value = +value;
                break;

            case '-':
                value = -value;
                break;

            case '~':
                value = ~value;
                break;
        }
        emit_insn(OP_LOAD_CONST, "%d", value);
        return operand_stack;
    }

    evaluate_express(lambda, express);
    switch (operator->token) {
        case '+':
            break;

        case '-':
            emit_insn(OP_NEG, NULL);
            break;

        case '~':
            emit_insn(OP_COM, NULL);
            break;

        case TOKEN_NOT:
            emit_insn(OP_NOT, NULL);
            break;
    }
    return operand_stack;
}

operand_t *compile_call_express(lambda_t *lambda, ast_t *ast)
{
    ast_t *callee = ast_get_child(ast, 0);
    ast_t *arg_list = ast_get_child(ast, 1);
    int arg_count = ast_child_count(arg_list);

    for (int i = arg_count - 1; i >= 0; i--) {
        ast_t *arg = ast_get_child(arg_list, i);
        evaluate_express(lambda, arg);
    }
    evaluate_express(lambda, callee);
    emit_insn(OP_CALL, "%i", arg_count);
    return operand_stack;
}

operand_t *compile_item_express(lambda_t *lambda, ast_t *ast)
{
    ast_t *array = ast_get_child(ast, 0);
    ast_t *index = ast_get_child(ast, 1);

    evaluate_express(lambda, index);
    evaluate_express(lambda, array);
    operand_t *item = operand_item_new();
    return item;
}

operand_t *compile_field_express(lambda_t *lambda, ast_t *ast)
{
    ast_t *object = ast_get_child(ast, 0);
    ast_t *name = ast_get_child(ast, 1);

    evaluate_express(lambda, object);
    operand_t *field = operand_field_new(name->svalue);
    return field;
}

operand_t *compile_postfix_express(lambda_t *lambda, ast_t *ast)
{
    return NULL;
}

operand_t *compile_token_id(lambda_t *lambda, ast_t *ast)
{
    char *name = ast->svalue;

    symbol_t *symbol = lookup_symbol(name);
    if (symbol == NULL)
        return operand_global_new(name);

    operand_t *operand = NULL;
    switch (symbol->type) {
        case SCOPE_GLOBAL:
            operand = operand_global_new(name);
            break;

        case SCOPE_LOCAL:
            operand = operand_local_new(symbol->offset);
            break;

        case SCOPE_CLASS:
            emit_insn(OP_LOAD_GLOBAL, "%S", scope_stack_top()->class_name);
            operand = operand_field_new(name);
            break;
    }
    return operand;
}

// What are the rules for local and global variables in Python?
//
// In Python, variables that are only referenced inside a function 
// are implicitly global. 
//
// If a variable is assigned a value anywhere within the function’s body, 
// it’s assumed to be a local unless explicitly declared as global.
operand_t *compile_token(lambda_t *lambda, ast_t *ast)
{
    assert(ast->type == AST_TOKEN);
    switch (ast->token) {
        case TOKEN_DOUBLE_CONST:
            return operand_double_new(ast->dvalue);  

        case TOKEN_STRING_CONST:
            return operand_string_new(ast->svalue);

        case TOKEN_ID:
            return compile_token_id(lambda, ast);

        case TOKEN_NONE:
            return operand_none;

        case TOKEN_TRUE:
            return operand_true;

        case TOKEN_FALSE:
            return operand_false;
    }
    return NULL;
}

operand_t *compile_list_express(lambda_t *lambda, ast_t *ast)
{
    emit_insn(OP_NEW_LIST, "%i", ast_child_count(ast));
    int i;
    ast_t *child;
    ast_each_child (ast, i, child) {
        evaluate_express(lambda, child);
        emit_insn(OP_LOAD_CONST, "%d", i);
        emit_insn(OP_DUP_TOP, "%i", 2);
        emit_insn(OP_STORE_ITEM, NULL);
    }
    return operand_stack;
}

operand_t *compile_dict_express(lambda_t *lambda, ast_t *ast)
{
    emit_insn(OP_NEW_DICT, NULL);
    int i;
    ast_t *child;
    ast_each_child (ast, i, child) {
        ast_t *key = ast_get_child(child, 0);
        ast_t *value = ast_get_child(child, 1);

        evaluate_express(lambda, value);
        evaluate_express(lambda, key);
        emit_insn(OP_DUP_TOP, "%i", 2);
        emit_insn(OP_STORE_ITEM, NULL);
    }
    return operand_stack;
}

operand_t *compile_tuple_express(lambda_t *lambda, ast_t *ast)
{
    for (int i = ast_child_count(ast) - 1; i >= 0; i--) {
        ast_t *child = ast_get_child(ast, i);
        evaluate_express(lambda, child);
    }

    emit_insn(OP_NEW_TUPLE, "%i", ast_child_count(ast));
    return operand_stack;
}

operand_t *compile_slice_express(lambda_t *lambda, ast_t *ast)
{
    ast_t *start = ast_get_child(ast, 0);
    ast_t *stop = ast_get_child(ast, 1);

    if (stop == NULL)
        emit_insn(OP_LOAD_CONST, "%o", operand_none);
    else
        evaluate_express(lambda, stop);
    evaluate_express(lambda, start);
    emit_insn(OP_NEW_SLICE, NULL);
    return operand_stack;
}

#define map(TYPE, type)                                                    \
    case AST_##TYPE:                                                       \
        operand = compile_##type(lambda, ast);                             \
        break
operand_t *compile_express(lambda_t *lambda, ast_t *ast)
{
    operand_t *operand = NULL;
    ast_stack_push(ast);

    switch (ast->type) {
        map(LAMBDA_EXPRESS, lambda_express);
        map(BINARY_EXPRESS, binary_express);
        map(UNARY_EXPRESS, unary_express);
        map(ITEM_EXPRESS, item_express);
        map(CALL_EXPRESS, call_express);
        map(FIELD_EXPRESS, field_express);
        map(POSTFIX_EXPRESS, postfix_express);
        map(TOKEN, token);
        map(LIST_EXPRESS, list_express);
        map(DICT_EXPRESS, dict_express);
        map(TUPLE_EXPRESS, tuple_express);
        map(SLICE_EXPRESS, slice_express);
        default:
            assert(false);
    }

    ast_stack_pop();
    return operand;
}
#undef map

operand_t *load_operand(lambda_t *lambda, operand_t *source)
{
    switch (source->type) {
        case OPERAND_IMMED:
        case OPERAND_STACK:
            return source;

        case OPERAND_DOUBLE:
        case OPERAND_STRING:
        case OPERAND_SYMBOL:
        case OPERAND_NONE:
        case OPERAND_TRUE:
        case OPERAND_FALSE:
            emit_insn(OP_LOAD_CONST, "%o", source);
            break;

        case OPERAND_GLOBAL:
            emit_insn(OP_LOAD_GLOBAL, "%S", source->svalue);
            break;

        case OPERAND_LOCAL:
            emit_insn(OP_LOAD_LOCAL, "%i", source->ivalue);
            break;

        case OPERAND_ITEM:
            emit_insn(OP_LOAD_ITEM, NULL);
            break;

        case OPERAND_FIELD:
            emit_insn(OP_LOAD_FIELD, "%S", source->svalue);
            break;

        default:
            assert(false);
    }

    operand_t *target = operand_stack;
    return target;
}

operand_t *evaluate_express(lambda_t *lambda, ast_t *ast)
{
    operand_t *source = compile_express(lambda, ast);
    operand_t *target = load_operand(lambda, source);
    return target;
}
