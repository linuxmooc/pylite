#include "root.h"
#include "branch.h"
#include "express.h"
#include "assign.h"
#include "loop.h"

void compile_break_clause(lambda_t *lambda, ast_t *ast)
{
    if (stack_is_empty(&lambda->break_stack))
        compile_error(ast, "break statement not within loop or switch.");
    operand_t *label = stack_top(&lambda->break_stack);
    emit_insn(OP_JMP, "%o", label);
}

void compile_continue_clause(lambda_t *lambda, ast_t *ast)
{
    if (stack_is_empty(&lambda->continue_stack))
        compile_error(ast, "continue statement not within loop.");
    operand_t *label = stack_top(&lambda->continue_stack);
    emit_insn(OP_JMP, "%o", label);
}

/**
 * Translation of while statement:
 *
 *         jmp cond
 * body:   ...
 *         ...
 * cond:   branch_true body 
 * exit:
 */
void compile_while_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *cond = ast_get_child(ast, 0);
    ast_t *body = ast_get_child(ast, 1);

    operand_t *body_label = new_label();
    operand_t *cond_label = new_label();
    operand_t *exit_label = new_label();

    stack_push(&lambda->break_stack, exit_label);
    stack_push(&lambda->continue_stack, cond_label);

    emit_insn(OP_JMP, "%o", cond_label);
    emit_label(body_label);
    compile_clause(lambda, body);
    emit_label(cond_label);
    branch_true(lambda, cond, body_label);
    emit_label(exit_label);

    stack_pop(&lambda->break_stack);
    stack_pop(&lambda->continue_stack);
}

/**
 * Translation of for statement:
 *
 *         iterator
 *         store_local temp
 *         jmp cond
 * body:   store to id
 *         ...
 * cond:   load_local temp
 *         next body
 *         pop StopIteration
 * exit:
 */
void compile_for_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *id = ast_get_child(ast, 0);
    ast_t *container = ast_get_child(ast, 1);
    ast_t *body = ast_get_child(ast, 2);

    operand_t *body_label = new_label();
    operand_t *cond_label = new_label();
    operand_t *exit_label = new_label();

    if (id->type == AST_TOKEN) 
        define_symbol(id->svalue);
    if (id->type == AST_TUPLE_EXPRESS)
        define_tuple_items(id);

    evaluate_express(lambda, container);
    int temp = lambda_new_local(lambda);
    emit_insn(OP_ITERATOR, NULL);
    emit_insn(OP_STORE_LOCAL, "%i", temp);

    stack_push(&lambda->break_stack, exit_label);
    stack_push(&lambda->continue_stack, cond_label);

    emit_insn(OP_JMP, "%o", cond_label);
    emit_label(body_label);
    if (id->type == AST_TUPLE_EXPRESS) {
        unpack_tuple(lambda, id);
        emit_insn(OP_POP, NULL);
    } else {
        operand_t *target = compile_express(lambda, id);
        store_operand(lambda, target);
    }
    compile_clause(lambda, body);

    emit_label(cond_label);
    emit_insn(OP_LOAD_LOCAL, "%i", temp);
    emit_insn(OP_NEXT, "%o", body_label);
    emit_insn(OP_POP, NULL);
    emit_label(exit_label);

    stack_pop(&lambda->break_stack);
    stack_pop(&lambda->continue_stack);
}
