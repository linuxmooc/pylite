#ifndef _COM_LEX_H
#define _COM_LEX_H

struct ast;

typedef struct {
    char *name;
    int value;
    struct ast *token;
} keyword_t; 

typedef struct {
    FILE *file;
    char *file_name;
    int line_number;

    int last_tabs;
    int tab_length;
    stack_template(struct ast*, token_stack);
} lex_t;

extern struct ast *lex_get_token(lex_t *this);
extern lex_t *lex_new(char *file_name);
extern void lex_dump(lex_t *this);

#endif
