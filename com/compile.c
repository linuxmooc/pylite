#include "root.h"
#include "compile.h"
#include "assign.h"
#include "loop.h"
#include "branch.h"
#include "express.h"
#include "except.h"
#include "trace.h"

void compile_error(ast_t *ast, char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    fprintf(stderr, "SyntaxError: ");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    fprintf(stderr, "  At %s, line %d\n", ast->file_name, ast->line_number);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void compile_clause(lambda_t *lambda, ast_t *ast);
void compile_clause_list(lambda_t *lambda, ast_t *ast);

void compile_express_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *express = ast_get_child(ast, 0);
    evaluate_express(lambda, express);
    emit_insn(OP_POP, NULL);
}

void compile_del_clause(lambda_t *lambda, ast_t *ast)
{
}

void compile_pass_clause(lambda_t *lambda, ast_t *ast)
{
}

void compile_import_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *id_list = ast_get_child(ast, 0);
    int i;
    ast_t *id;

    ast_each_child (id_list, i, id) {
        char *name = id->svalue;
        emit_insn(OP_LOAD_MODULE, "%S", name);
        emit_insn(OP_LOAD_THIS_MODULE, NULL);
        emit_insn(OP_STORE_FIELD, "%S", name);
    }
}

void compile_from_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *module = ast_get_child(ast, 0);
    ast_t *id_list = ast_get_child(ast, 1);
    int i;
    ast_t *id;

    ast_each_child (id_list, i, id) {
        emit_insn(OP_LOAD_MODULE, "%S", module->svalue);
        emit_insn(OP_LOAD_FIELD, "%S", id->svalue);

        emit_insn(OP_LOAD_THIS_MODULE, NULL);
        emit_insn(OP_STORE_FIELD, "%S", id->svalue);
    }
}

void compile_global_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *id_list = ast_get_child(ast, 0);
    int i;
    ast_t *id;

    ast_each_child (id_list, i, id)
        insert_global_symbol(id->svalue);
}

void compile_print_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *express = ast_get_child(ast, 0);
    if (express == NULL)
        emit_insn(OP_LOAD_CONST, "%s", "");
    else
        evaluate_express(lambda, express);
    emit_insn(OP_PRINT, NULL);
}

void compile_assert_clause(lambda_t *lambda, ast_t *ast)
{
}

void compile_return_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *express = ast_get_child(ast, 0);
    if (express)
        evaluate_express(lambda, express);
    else
        emit_insn(OP_LOAD_CONST, "%o", operand_none);

    if (!stack_is_empty(&lambda->finally_stack)) {
        int temp = lambda_new_local(lambda);
        emit_insn(OP_STORE_LOCAL, "%i", temp);

        operand_t *label = stack_top(&lambda->finally_stack);
        emit_insn(OP_FINALLY_CALL, "%o", label);
        
        emit_insn(OP_LOAD_LOCAL, "%i", temp);
    }

    emit_insn(OP_RETURN, NULL);
}

void compile_group_clause(lambda_t *lambda, ast_t *ast)
{
}

/**
 * Translation of if clause
 *              branch_false cond else_label
 *              then_part
 *              jump exit_label
 * else_label:  ... 
 * exit_label:
 */
void build_if_clause(lambda_t *lambda, ast_t *ast, operand_t *exit_label)
{
    assert(ast->type == AST_IF_CLAUSE || ast->type == AST_ELIF_CLAUSE);
    ast_t *cond = ast_get_child(ast, 0);
    ast_t *then_part = ast_get_child(ast, 1);

    operand_t *else_label = new_label();
    branch_false(lambda, cond, else_label);
    compile_clause(lambda, then_part);
    emit_insn(OP_JMP, "%o", exit_label);
    emit_label(else_label);
}

void compile_if_clause(lambda_t *lambda, ast_t *ast)
{
    operand_t *exit_label = new_label();
    build_if_clause(lambda, ast, exit_label);

    for (int i = 2; i < ast_child_count(ast); i++) {
        ast_t *child = ast_get_child(ast, i);

        if (child->type == AST_ELIF_CLAUSE) {
            build_if_clause(lambda, child, exit_label);
            continue;
        }

        if (child->type == AST_ELSE_CLAUSE) {
            ast_t *else_part = ast_get_child(child, 0);
            compile_clause(lambda, else_part);
            continue;
        }
    }

    emit_label(exit_label);
}

void compile_elif_clause(lambda_t *lambda, ast_t *ast)
{
}

void compile_else_clause(lambda_t *lambda, ast_t *ast)
{
}

void compile_param_list(lambda_t *def, ast_t *param_list)
{
    int i;
    ast_t *param;

    ast_each_child (param_list, i, param) 
        insert_param_symbol(param->svalue);
}

void compile_def_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *id = ast_get_child(ast, 0);
    ast_t *param_list = ast_get_child(ast, 1);
    ast_t *clause_list = ast_get_child(ast, 2);

    char *name = id->svalue;
    lambda_t *def = lambda_new(name);
    define_symbol(name);

    enter_local_scope(def);
    compile_param_list(def, param_list);
    compile_clause_list(def, clause_list);
    leave_local_scope();

    // Add return clause
    ast_t *last_clause = ast_get_last_child(clause_list);
    if ((last_clause == NULL) || (last_clause->type != AST_RETURN_CLAUSE)) {
        lambda_emit_insn(def, OP_LOAD_CONST, "%o", operand_none);
        lambda_emit_insn(def, OP_RETURN, NULL);
    }

    emit_insn(OP_LOAD_CONST, "%o", def);
    operand_t *target = compile_express(lambda, id);
    store_operand(lambda, target);
}

void compile_class_clause(lambda_t *lambda, ast_t *ast)
{
    ast_t *id = ast_get_child(ast, 0);
    ast_t *parent = ast_get_child(ast, 1);
    ast_t *clause_list = ast_get_child(ast, 2);

    char *name = id->svalue;
    define_symbol(name);
    emit_insn(OP_NEW_CLASS, "%S", name);
    operand_t *target = compile_express(lambda, id);
    store_operand(lambda, target);

    if (parent != NULL) {
        evaluate_express(lambda, parent);
        evaluate_express(lambda, id);
        emit_insn(OP_INHERIT, NULL);
    }

    enter_class_scope(id->svalue);
    compile_clause_list(lambda, clause_list);
    leave_class_scope();
}

void compile_clause_list(lambda_t *lambda, ast_t *ast)
{
    int i;
    ast_t *child;

    ast_each_child (ast, i, child)
        compile_clause(lambda, child);
}

#define map(TYPE, type)                                                      \
    case AST_##TYPE:                                                         \
        compile_##type(lambda, ast);                                         \
        break
void compile_clause(lambda_t *lambda, ast_t *ast)
{
    ast_stack_push(ast);
    switch (ast->type) {
        map(CLAUSE_LIST, clause_list);
        map(EXPRESS_CLAUSE, express_clause);
        map(ASSIGN_CLAUSE, assign_clause);
        map(DEL_CLAUSE, del_clause);
        map(PASS_CLAUSE, pass_clause);
        map(IMPORT_CLAUSE, import_clause);
        map(FROM_CLAUSE, from_clause);
        map(GLOBAL_CLAUSE, global_clause);
        map(PRINT_CLAUSE, print_clause);
        map(ASSERT_CLAUSE, assert_clause);
        map(BREAK_CLAUSE, break_clause);
        map(CONTINUE_CLAUSE, continue_clause);
        map(RETURN_CLAUSE, return_clause);
        map(GROUP_CLAUSE, group_clause);
        map(IF_CLAUSE, if_clause);
        map(ELIF_CLAUSE, elif_clause);
        map(ELSE_CLAUSE, else_clause);
        map(WHILE_CLAUSE, while_clause);
        map(FOR_CLAUSE, for_clause);
        map(TRY_CLAUSE, try_clause);
        map(RAISE_CLAUSE, raise_clause);
        map(DEF_CLAUSE, def_clause);
        map(CLASS_CLAUSE, class_clause);
        default:
            assert(false);
    }
    ast_stack_pop();
}
#undef map

lambda_t *compile_module(ast_t *ast)
{
    lambda_t *lambda = lambda_new("__init__");
    enter_global_scope();
    ast_stack_push(ast);

    compile_clause_list(lambda, ast);
    emit_insn(OP_LOAD_CONST, "%o", operand_none);
    emit_insn(OP_RETURN, NULL);

    ast_stack_pop();
    leave_global_scope();
    return lambda;
}
