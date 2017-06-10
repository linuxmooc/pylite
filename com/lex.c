#include "root.h"
#include "lex.h"
#include "ast.h"
#include "gram.tab.h"

void yyerror(lex_t *lex, ast_t **module, char *msg)
{
    fprintf(stderr, "SyntaxError:\n");
    fprintf(stderr, "  At %s, line %d\n", lex->file_name, lex->line_number);
    exit(EXIT_FAILURE);
}

void lex_error(lex_t *this, char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    fprintf(stderr, "SyntaxError: ");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    fprintf(stderr, "  At %s, line %d\n", this->file_name, this->line_number);
    va_end(ap);

    exit(EXIT_FAILURE);
}

keyword_t keyword_array[] = {
    {"if",         TOKEN_IF},
    {"elif",       TOKEN_ELIF},
    {"else",       TOKEN_ELSE},
    {"for",        TOKEN_FOR},
    {"while",      TOKEN_WHILE},
    {"in",         TOKEN_IN},
    {"continue",   TOKEN_CONTINUE},
    {"break",      TOKEN_BREAK},
    {"return",     TOKEN_RETURN},
    {"global",     TOKEN_GLOBAL},
    {"from",       TOKEN_FROM},
    {"import",     TOKEN_IMPORT},
    {"del",        TOKEN_DEL},
    {"pass",       TOKEN_PASS},
    {"print",      TOKEN_PRINT},
    {"assert",     TOKEN_ASSERT},
    {"try",        TOKEN_TRY},
    {"except",     TOKEN_EXCEPT},
    {"as",         TOKEN_AS},
    {"finally",    TOKEN_FINALLY},
    {"raise",      TOKEN_RAISE},
    {"class",      TOKEN_CLASS},
    {"def",        TOKEN_DEF},
    {"lambda",     TOKEN_LAMBDA},

    {"None",       TOKEN_NONE},
    {"True",       TOKEN_TRUE},
    {"False",      TOKEN_FALSE},
    {"not",        TOKEN_NOT},
    {"and",        TOKEN_AND},
    {"or",         TOKEN_OR},

    {"==",         TOKEN_EQ},
    {"!=",         TOKEN_NE},
    {">=",         TOKEN_GE},
    {"<=",         TOKEN_LE},
    {"<<",         TOKEN_LSH},
    {">>",         TOKEN_RSH},

    {"|=",         TOKEN_OR_ASSIGN},
    {"^=",         TOKEN_XOR_ASSIGN},
    {"&=",         TOKEN_AND_ASSIGN},
    {"+=",         TOKEN_ADD_ASSIGN},
    {"-=",         TOKEN_SUB_ASSIGN},
    {"*=",         TOKEN_MUL_ASSIGN},
    {"/=",         TOKEN_DIV_ASSIGN},
    {"%=",         TOKEN_MOD_ASSIGN},
    {"<<=",        TOKEN_LSH_ASSIGN},
    {">>=",        TOKEN_RSH_ASSIGN},

    {"\n",         '\n'},
    {"?",          '?'},
    {":",          ':'},
    {"=",          '='},
    {"|",          '|'},
    {"^",          '^'},
    {"&",          '&'},
    {"<",          '<'},
    {">",          '>'},
    {"+",          '+'},
    {"-",          '-'},
    {"*",          '*'},
    {"/",          '/'},
    {"%",          '%'},
    {"!",          '!'},
    {"~",          '~'},
    {".",          '.'},
    {",",          ','},
    {";",          ';'},
    {"(",          '('},
    {")",          ')'},
    {"{",          '{'},
    {"}",          '}'},
    {"[",          '['},
    {"]",          ']'},
};

#define KEYWORD_COUNT array_count(keyword_array)
char *operator_chars = "\n?:=|^&<>+-*/%!~.,;(){}[]";

int lex_is_operator(int c)
{
    return (c < 256) && strchr(operator_chars, c);
}

lex_t *lex_new(char *file_name)
{
    lex_t *this = malloc(sizeof(lex_t));
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "minipy: can't open file %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    this->file = file;
    this->file_name = file_name;
    this->line_number = 1;

    this->last_tabs = 0;
    this->tab_length = 4;
    stack_init(&this->token_stack);
    return this;
}

ast_t *lex_get_keyword(lex_t *this, char *name)
{
    int i;

    for (i = 0; i < KEYWORD_COUNT; i++) {
        keyword_t *keyword = keyword_array + i;
        if (strcmp(keyword->name, name) == 0) {
            ast_t *token = ast_new_token(keyword->value);
            token->svalue = keyword->name;
            return token;
        }
    }
    return NULL;
}

int lex_get_char(lex_t *this)
{
    int c = fgetc(this->file);
    if (c == '\n')
        this->line_number++;
    return c;
}

void lex_unget_char(lex_t *this, int c)
{
    if (c == EOF)
        return;
    if (c == '\n')
        this->line_number--;
    ungetc(c, this->file);
}

int lex_peek_char(lex_t *this)
{
    int c = lex_get_char(this);
    lex_unget_char(this, c);
    return c;
}

void lex_skip_spaces(lex_t *this)
{
    int c;

    while (c = lex_get_char(this), isblank(c))
        ;
    lex_unget_char(this, c);
}

void lex_skip_garbage(lex_t *this)
{
    int c;

    while (c = lex_get_char(this), isblank(c))
        ;

    if (c == '#') {
        while (c = lex_get_char(this), (c != EOF && c != '\n'))
            ;
    }

    lex_unget_char(this, c);
}

ast_t *lex_get_word(lex_t *this)
{
    int c = lex_get_char(this);
    if (!(isalpha(c) || c == '_')) {
        lex_unget_char(this, c);
        return NULL;
    }

    text_t text;
    text_init(&text);
    text_put_char(&text, c);
    while (c = lex_get_char(this), (isalnum(c) || c == '_'))
        text_put_char(&text, c);
    lex_unget_char(this, c);

    ast_t *token = lex_get_keyword(this, text.data);
    if (token) {
        text_destroy(&text);
        return token;
    }

    token = ast_new_token(TOKEN_ID);
    token->svalue = strdup(text.data);
    text_destroy(&text);
    return token;
}

ast_t *lex_get_double(lex_t *this)
{
    int sign = +1;
    int c = lex_get_char(this);
    if (!isdigit(c)) {
        lex_unget_char(this, c);
        return 0;
    }

    text_t text;
    text_init(&text);
    text_put_char(&text, c);
    while (c = lex_get_char(this), (isdigit(c) || c == '.'))
        text_put_char(&text, c);
    lex_unget_char(this, c);

    ast_t *token;
    token = ast_new_token(TOKEN_DOUBLE_CONST);
    token->dvalue = atof(text.data);
    text_destroy(&text);
    return token;
}

static bool is_quote(int c)
{
    return c == '"' || c == '\'';
}

ast_t *lex_get_string(lex_t *this)
{
    int c = lex_get_char(this);
    if (!is_quote(c)) {
        lex_unget_char(this, c);
        return 0;
    }

    text_t text;
    text_init(&text);
    while (c = lex_get_char(this), (c != EOF && !is_quote(c)))
        text_put_char(&text, c);
    assert(c != EOF);

    ast_t *token = ast_new_token(TOKEN_STRING_CONST);
    token->svalue = strdup(text.data);
    text_destroy(&text);
    return token;
}

ast_t *lex_get_operator(lex_t *this)
{
    int c = lex_get_char(this);
    if (!lex_is_operator(c)) {
        lex_unget_char(this, c);
        return NULL;
    }

    text_t text;
    text_init(&text);
    text_put_char(&text, c);
    while (c = lex_get_char(this), lex_is_operator(c))
        text_put_char(&text, c);
    lex_unget_char(this, c);

    while (true) {
        ast_t *token = lex_get_keyword(this, text.data);
        if (token)
            return token;
        c = text_trim_char(&text);
        lex_unget_char(this, c);
    }
    text_destroy(&text);
    return NULL;
}

int lex_guess_tabs(lex_t *this, text_t *text)
{
    int tabs = 0;
    int spaces = 0;

    for (char *p = text->data; p < text->data + text->size; p++) {
        switch (*p) {
            case '\t':
                tabs++;
                break;

            case ' ':
                spaces++;
                break;

            default:
                assert(0);
        } 
    }

    if (spaces && tabs)
        lex_error(this, "mix spaces and tabs");

    if (spaces)
        return spaces / this->tab_length; 
    return tabs;
}

void lex_read_tabs(lex_t *this)
{
    int c;
    text_t text;

    text_init(&text);
    while (c = lex_get_char(this), isblank(c))
        text_put_char(&text, c);
    lex_unget_char(this, c);
    int this_tabs = lex_guess_tabs(this, &text);
    int last_tabs = this->last_tabs; 
    text_destroy(&text);

    /* Ignore empty line */
    if (c == '\n')
        return;

    /* Ignore comment */
    if (c == '#')
        return;

    int count;
    ast_t *token;
    if (this_tabs > last_tabs) {
        count = this_tabs - last_tabs;
        token = ast_new_token(TOKEN_BEGIN);
    } else {
        count = last_tabs - this_tabs;
        token = ast_new_token(TOKEN_END);
    }

    for (int i = 0; i < count; i++)
        stack_push(&this->token_stack, token);
    this->last_tabs = this_tabs; 
}

ast_t *lex_meet_eof(lex_t *this)
{
    for (int i = 0; i < this->last_tabs; i++) {
        ast_t *token = ast_new_token(TOKEN_BEGIN);
        stack_push(&this->token_stack, token);
    }
    this->last_tabs = 0;

    if (stack_is_empty(&this->token_stack))
        return NULL;
    else
        return stack_pop(&this->token_stack);
}

ast_t *lex_fetch_token(lex_t *this)
{
    ast_t *token;

    if (!stack_is_empty(&this->token_stack))
        return stack_pop(&this->token_stack);

    int c;
    c = lex_peek_char(this);
    if (c == EOF)
        return lex_meet_eof(this);

    lex_skip_garbage(this);
    c = lex_get_char(this);
    if (c == '\n') {
        lex_read_tabs(this);
        return ast_new_token('\n');
    }
    lex_unget_char(this, c);

    if (token = lex_get_word(this)) 
        return token;
    if (token = lex_get_operator(this))
        return token;
    if (token = lex_get_double(this))
        return token;
    if (token = lex_get_string(this))
        return token;

    lex_error(this, "unknown character");
    return NULL;
}

ast_t *lex_get_token(lex_t *this)
{
    ast_t *token = lex_fetch_token(this);
    if (token != NULL) {
        token->file_name = this->file_name;
        token->line_number = this->line_number;
    }
    return token;
}

void token_dump(ast_t *ast)
{
    int token = ast->token;

    if (lex_is_operator(token)) {
        if (token == '\n') 
            printf("OP   \\n\n");
        else
            printf("OP   %c\n", token);
        return;
    }

    for (int i = 0; i < KEYWORD_COUNT; i++) {
        keyword_t *keyword = keyword_array + i; 
        if (keyword->value == token) {
            printf("KEY  %s\n", keyword->name);
            return;
        }
    }
 
    switch (token) {
        case TOKEN_ID:
            printf("ID      %s\n", ast->svalue);
            break;

        case TOKEN_DOUBLE_CONST:
            printf("NUMBER  %lf\n", ast->dvalue);
            break;

        case TOKEN_STRING_CONST:
            printf("STRING  \"%s\"\n", ast->svalue);
            break;

        case TOKEN_BEGIN:
            puts("BEGIN");
            break;

        case TOKEN_END:
            puts("END");
            break;

        default:
            assert(0);
    }
}

void lex_dump(lex_t *this)
{
    ast_t *token;

    while (token = lex_get_token(this))
        token_dump(token);
}
