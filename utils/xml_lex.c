#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text.h"
#include "xml_lex.h"

xml_token_t *xml_token_new(int type, char *value)
{
    xml_token_t *this = malloc(sizeof(xml_token_t));
    this->type = type;
    switch (type) {
        case XML_TOKEN_NEWLINE:
        case XML_TOKEN_STRING:
        case XML_TOKEN_SYMBOL:
            this->svalue = strdup(value);
            break;

        case XML_TOKEN_DOUBLE:
            this->dvalue = strtod(value, NULL);
            break;

        case XML_TOKEN_PUSH:
        case XML_TOKEN_POP:
            this->svalue = NULL;
            break;

        defualt:
            assert(0);
    }
    return this;
}

void xml_token_dump(xml_token_t *this)
{
    switch (this->type) {
        case XML_TOKEN_DOUBLE:
            printf("DOUBLE  %lf\n", this->dvalue);
            break;

        case XML_TOKEN_STRING:
            printf("STRING  \"%s\"\n", this->svalue);
            break;

        case XML_TOKEN_SYMBOL:
            printf("SYMBOL  \"%s\"\n", this->svalue);
            break;

        case XML_TOKEN_PUSH:
            puts("PUSH  {");
            break;

        case XML_TOKEN_POP:
            puts("POP  }");
            break;

        case XML_TOKEN_NEWLINE:
            puts("NEWLINE");
            break;

        default:
            assert(0);
    }
}

void xml_lex_error(xml_lex_t *this, char *format, ...)
{
    fprintf(stderr, "%s:%d: error: ", this->file_name, this->line_number);

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

xml_lex_t *xml_lex_new(char *file_name)
{
    xml_lex_t *this = malloc(sizeof(xml_lex_t));
    FILE *file = fopen(file_name, "r");
    if (!file)
        return NULL;

    this->file = file;
    this->file_name = file_name;
    this->line_number = 1;

    this->last_tabs = 0;
    this->tab_length = 2;
    stack_init(&this->token_stack);
    return this;
}

int xml_lex_get_char(xml_lex_t *this)
{
    int c = fgetc(this->file);
    if (c == '\n')
        this->line_number++;
    return c;
}

void xml_lex_unget_char(xml_lex_t *this, int c)
{
    if (c == EOF)
        return;
    if (c == '\n')
        this->line_number--;
    ungetc(c, this->file);
}

int xml_lex_peek_char(xml_lex_t *this)
{
    int c = xml_lex_get_char(this);
    xml_lex_unget_char(this, c);
    return c;
}

void xml_lex_skip_spaces(xml_lex_t *this)
{
    int c;

    while (c = xml_lex_get_char(this), isblank(c))
        ;
    xml_lex_unget_char(this, c);
}

xml_token_t *xml_lex_get_word(xml_lex_t *this)
{
    int c = xml_lex_get_char(this);
    if (!(isalpha(c) || c == '_')) {
        xml_lex_unget_char(this, c);
        return NULL;
    }

    text_t text;
    text_init(&text);
    text_put_char(&text, c);
    while (c = xml_lex_get_char(this), (isalnum(c) || c == '_'))
        text_put_char(&text, c);
    xml_lex_unget_char(this, c);

    xml_token_t *token = xml_token_new(XML_TOKEN_SYMBOL, text.data);
    text_destroy(&text);
    return token;
}

static bool is_digit(int c)
{
    return c == '-' || c == '.' || isdigit(c);
}

xml_token_t *xml_lex_get_double(xml_lex_t *this)
{
    int c = xml_lex_get_char(this);
    if (!is_digit(c)) {
        xml_lex_unget_char(this, c);
        return 0;
    }

    text_t text;
    text_init(&text);
    text_put_char(&text, c);
    while (c = xml_lex_get_char(this), is_digit(c))
        text_put_char(&text, c);
    xml_lex_unget_char(this, c);

    xml_token_t *token = xml_token_new(XML_TOKEN_DOUBLE, text.data);
    text_destroy(&text);
    return token;
}

xml_token_t *xml_lex_get_string(xml_lex_t *this)
{
    int c = xml_lex_get_char(this);
    if (c != '\'' && c != '"') {
        xml_lex_unget_char(this, c);
        return 0;
    }
    int quote = c;

    text_t text;
    text_init(&text);
    while (c = xml_lex_get_char(this), (c != EOF && c != quote)) {
        if (c == '\\') {
            int d = xml_lex_get_char(this);
            switch (d) {
                case 'n':
                    c = '\n';
                    break;

                case 'r':
                    c = '\r';
                    break;

                case 't':
                    c = '\t';
                    break;

                default:
                    c = d;
            }
        }
        text_put_char(&text, c);
    }
    assert(c == quote);

    xml_token_t *token = xml_token_new(XML_TOKEN_STRING, text.data);
    token->svalue = strdup(text.data);
    text_destroy(&text);
    return token;
}

int xml_lex_count_tabs(xml_lex_t *this, text_t *text)
{
    int spaces = 0;

    for (char *p = text->data; p < text->data + text->size; p++) {
        switch (*p) {
            case ' ':
                spaces++;
                break;

            default:
                assert(0);
        } 
    }

    return spaces / this->tab_length; 
}

void xml_lex_read_tabs(xml_lex_t *this)
{
    int c;
    text_t text;

    text_init(&text);
    while (c = xml_lex_get_char(this), isblank(c))
        text_put_char(&text, c);
    xml_lex_unget_char(this, c);
    int this_tabs = xml_lex_count_tabs(this, &text);
    int last_tabs = this->last_tabs; 
    text_destroy(&text);

    // Ignore empty line
    if (c == '\n')
        return;

    int count;
    xml_token_t *token;
    if (this_tabs > last_tabs) {
        count = this_tabs - last_tabs;
        token = xml_token_new(XML_TOKEN_PUSH, NULL);
    } else {
        count = last_tabs - this_tabs;
        token = xml_token_new(XML_TOKEN_POP, NULL);
    }

    for (int i = 0; i < count; i++)
        stack_push(&this->token_stack, token);
    this->last_tabs = this_tabs; 
}

xml_token_t *xml_lex_meet_eof(xml_lex_t *this)
{
    for (int i = 0; i < this->last_tabs; i++) {
        xml_token_t *token = xml_token_new(XML_TOKEN_PUSH, NULL);
        stack_push(&this->token_stack, token);
    }
    this->last_tabs = 0;

    if (stack_is_empty(&this->token_stack))
        return NULL;
    else
        return stack_pop(&this->token_stack);
}

xml_token_t *xml_lex_get_token(xml_lex_t *this)
{
    xml_token_t *token;

    if (!stack_is_empty(&this->token_stack))
        return stack_pop(&this->token_stack);

    int c;
    c = xml_lex_peek_char(this);
    if (c == EOF)
        return xml_lex_meet_eof(this);

    xml_lex_skip_spaces(this);
    c = xml_lex_get_char(this);
    if (c == '\n') {
        xml_lex_read_tabs(this);
        return xml_token_new(XML_TOKEN_NEWLINE, "\\n");
    }
    xml_lex_unget_char(this, c);

    if (token = xml_lex_get_word(this)) 
        return token;
    if (token = xml_lex_get_double(this))
        return token;
    if (token = xml_lex_get_string(this))
        return token;

    xml_lex_error(this, "unknown character '%c'\n", c);
    return NULL;
}

void xml_lex_unget_token(xml_lex_t *this, xml_token_t *token)
{
    stack_push(&this->token_stack, token);
}

void xml_lex_dump(xml_lex_t *this)
{
    xml_token_t *token;

    while (token = xml_lex_get_token(this))
        xml_token_dump(token);
}
