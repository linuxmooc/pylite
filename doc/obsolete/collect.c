void collect_from_global(ast_t *ast)
{
    ast_t *id_list = ast_get_child(ast, 0);
    for (int i = 0; i < ast_child_count(id_list); i++) {
        ast_t *id = ast_get_child(id_list, i);
        insert_global_symbol(id->svalue);
    }
}

void collect_from_assign(ast_t *ast)
{
    ast_t *left = ast_get_child(ast, 0);
    ast_t *right = ast_get_child(ast, 2);
    insert_local_symbol(left->svalue);
    collect_symbol(right);
}

void collect_from_list(ast_t *ast)
{
    for (int i = 0; i < ast_child_count(ast); i++) {
        ast_t *child = ast_get_child(ast, i);
        collect_symbol(child);
    }
}

void collect_symbol(ast_t *ast)
{
    switch (ast->type) {
        case AST_GLOBAL_CLAUSE:
            collect_from_global(ast);
            break;

        case AST_ASSIGN_CLAUSE:
            collect_from_assign(ast);
            break;

        case AST_CLAUSE_LIST:
            collect_from_list(ast);
            break;
    }
}
