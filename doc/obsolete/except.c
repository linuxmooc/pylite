#include "root.h"
#include "except.h"
#include "express.h"

void compile_raise_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *express = ast_get_child(ast, 0);
    evaluate_express(lambda, express);
    emit_insn(OP_RAISE, NULL);
}

void compile_except_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *handler = ast_get_child(ast, 0);
    compile_clause(lambda, handler);
}

/**
 * Translation of try-except clause
 *
 * start:   try_body
 * end:     ...
 *          jmp exit
 * handler: pop
 *          except_body
 *          ...
 * exit:    ...
 */
void try_except(lambda_t *lambda, ast_t *try_clause, ast_t *except_clause)
{
    operand_t *start_label = new_label();
    operand_t *end_label = new_label();
    operand_t *handler_label = new_label();
    operand_t *exit_label = new_label();
    
    guard_t guard;
    guard.start = start_label;
    guard.end = end_label;
    guard.handler = handler_label;
    vector_push_back(&lambda->guard_vector, guard);

    emit_label(start_label);
    compile_clause(lambda, try_clause);
    emit_label(end_label);
    emit_insn(OP_JMP, "%o", exit_label);

    emit_label(handler_label);
    emit_insn(OP_POP, NULL);
    compile_except_clause(lambda, except_clause);
    emit_label(exit_label);
}

void compile_finally_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *handler = ast_get_child(ast, 0);
    compile_clause(lambda, handler);
}

/**
 * Translation of try-finally clause
 *
 * start:   try_body
 * end:     ...
 *          jmp exit
 * handler: store_local temp 
 *          finally_body
 *          finally_exit temp
 * exit:    finally_call
 */
void try_finally(lambda_t *lambda, ast_t *try_clause, ast_t *finally_clause)
{
    operand_t *start_label = new_label();
    operand_t *end_label = new_label();
    operand_t *handler_label = new_label();
    operand_t *exit_label = new_label();
    
    guard_t guard;
    guard.start = start_label;
    guard.end = end_label;
    guard.handler = handler_label;
    vector_push_back(&lambda->guard_vector, guard);

    stack_push(&lambda->finally_stack, handler_label);
    emit_label(start_label);
    compile_clause(lambda, try_clause);
    emit_label(end_label);
    emit_insn(OP_JMP, "%o", exit_label);
    stack_pop(&lambda->finally_stack);

    int temp = lambda_new_local(lambda);
    emit_label(handler_label);
    emit_insn(OP_STORE_LOCAL, "%i", temp);
    compile_finally_clause(lambda, finally_clause);
    emit_insn(OP_FINALLY_EXIT, "%i", temp);

    emit_label(exit_label);
    emit_insn(OP_FINALLY_CALL, "%o", handler_label);
}

void compile_try_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *try_clause = ast_get_child(ast, 0);
    ast_t *except_clause = ast_get_child(ast, 1);
    ast_t *finally_clause = ast_get_child(ast, 2);

    if (except_clause != NULL && finally_clause == NULL) {
        try_except(lambda, try_clause, except_clause);
        return;
    }

    if (except_clause == NULL && finally_clause != NULL) {
        try_finally(lambda, try_clause, finally_clause);
        return;
    }

    assert(false);
}
