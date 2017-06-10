#ifndef _COM_AST_H
#define _COM_AST_H

enum {
    /* 00 */  AST_MODULE,
    /* 01 */  AST_CLAUSE_LIST,
    /* 02 */  AST_CLAUSE,
    /* 03 */  AST_EXPRESS_CLAUSE,
    /* 04 */  AST_ASSIGN_CLAUSE,
    /* 05 */  AST_DEL_CLAUSE,
    /* 06 */  AST_PASS_CLAUSE,
    /* 07 */  AST_IMPORT_CLAUSE,
    /* 08 */  AST_FROM_CLAUSE,
    /* 09 */  AST_GLOBAL_CLAUSE,
    /* 10 */  AST_PRINT_CLAUSE,
    /* 11 */  AST_ASSERT_CLAUSE,
    /* 12 */  AST_BREAK_CLAUSE,
    /* 13 */  AST_CONTINUE_CLAUSE,
    /* 14 */  AST_RETURN_CLAUSE,
    /* 15 */  AST_GROUP_CLAUSE,
    /* 16 */  AST_IF_CLAUSE,
    /* 17 */  AST_ELIF_CLAUSE,
    /* 18 */  AST_ELSE_CLAUSE,
    /* 19 */  AST_WHILE_CLAUSE,
    /* 20 */  AST_FOR_CLAUSE,
    /* 21 */  AST_TRY_CLAUSE,
    /* 22 */  AST_EXCEPT_CLAUSE,
    /* 23 */  AST_FINALLY_CLAUSE,
    /* 24 */  AST_RAISE_CLAUSE,
    /* 25 */  AST_DEF_CLAUSE,
    /* 26 */  AST_PARAM_LIST,
    /* 27 */  AST_CLASS_CLAUSE,
    /* 28 */  AST_LAMBDA_EXPRESS,
    /* 29 */  AST_SLICE_EXPRESS,
    /* 30 */  AST_BINARY_EXPRESS,
    /* 31 */  AST_UNARY_EXPRESS,
    /* 32 */  AST_CALL_EXPRESS,
    /* 33 */  AST_ARG_LIST,
    /* 34 */  AST_ID_LIST,
    /* 35 */  AST_ITEM_EXPRESS,
    /* 36 */  AST_FIELD_EXPRESS,
    /* 37 */  AST_POSTFIX_EXPRESS,
    /* 38 */  AST_TOKEN,
    /* 39 */  AST_LIST_EXPRESS,
    /* 40 */  AST_DICT_EXPRESS,
    /* 41 */  AST_DICT_ITEM,
    /* 42 */  AST_TUPLE_EXPRESS,
              AST_NUMBER
};

typedef struct ast {
    int type;
    char *file_name;
    int line_number;

    // type == AST_TOKEN
    // TOKEN_ID, svalue
    // TOKEN_DOUBLE_CONST, dvalue 
    // TOKEN_STRING_CONST, svalue
    int token;
    union {
        double dvalue;
        char *svalue;
    };
    vector_template(struct ast *, child_vector);
} ast_t;

#define ast_each_child(ast, i, child) \
    vector_each(&ast->child_vector, i, child)

extern ast_t *ast_add_child(ast_t *this, ast_t *child);
extern ast_t *ast_prepend_child(ast_t *this, ast_t *child);
extern ast_t *ast_new(int type, int count, ...);
extern ast_t *ast_new_token(int token);
extern int ast_is_token(ast_t *this, int token);
extern void ast_dump(ast_t *this, xml_file_t *xml_file);
extern char *ast_name(ast_t *this);

extern int ast_child_count(ast_t *this);
extern ast_t *ast_get_child(ast_t *this, int index);
extern void ast_set_child(ast_t *this, int index, ast_t *child);
extern ast_t *ast_get_last_child(ast_t *this);
extern void ast_locate(ast_t *this, char *file_name);

#endif
