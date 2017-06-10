#ifndef _XML_LEX_H
#define _XML_LEX_H

#include "stack.h"

enum {
    XML_TOKEN_PUSH,
    XML_TOKEN_POP,
    XML_TOKEN_NEWLINE,
    XML_TOKEN_STRING,
    XML_TOKEN_SYMBOL,
    XML_TOKEN_DOUBLE
};

typedef struct {
    int type;
    union {
        double dvalue;
        char *svalue;
    };
} xml_token_t;
extern xml_token_t *xml_token_new(int type, char *value);

typedef struct {
    FILE *file;
    char *file_name;
    int line_number;
    int last_tabs;
    int tab_length;
    stack_template(xml_token_t *, token_stack);
} xml_lex_t;

extern xml_token_t *xml_lex_get_token(xml_lex_t *this);
extern void xml_lex_unget_token(xml_lex_t *this, xml_token_t *token);
extern xml_lex_t *xml_lex_new(char *file_name);
extern void xml_lex_dump(xml_lex_t *this);

#endif
