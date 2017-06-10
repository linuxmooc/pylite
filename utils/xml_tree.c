#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "xml_tree.h"
#include "xml_file.h"
#include "xml_lex.h"

xml_tree_t *xml_tree_new()
{
    xml_tree_t *this = malloc(sizeof(xml_tree_t));
    vector_init(&this->token_vector);
    vector_init(&this->child_vector);
    return this;
}

xml_tree_t *xml_tree_get_child(xml_tree_t *this, int index)
{
    xml_tree_t *xml_child = vector_get(&this->child_vector, index);
    return xml_child;
}

xml_token_t *xml_tree_get_token(xml_tree_t *this, int index)
{
    xml_token_t *xml_token = vector_get(&this->token_vector, index);
    return xml_token;
}

char *xml_tree_get_tag(xml_tree_t *this)
{
    xml_token_t *xml_token = xml_tree_get_token(this, 0);
    assert(xml_token->type == XML_TOKEN_SYMBOL);
    return xml_token->svalue;
}

void xml_tree_add_child(xml_tree_t *this, xml_tree_t *xml_child)
{
    vector_push_back(&this->child_vector, xml_child);
}

void xml_tree_add_token(xml_tree_t *this, xml_token_t *xml_token)
{
    vector_push_back(&this->token_vector, xml_token);
}

xml_tree_t *xml_tree_find_child(xml_tree_t *this, char *tag)
{
    int i;
    xml_tree_t *xml_child;
    vector_each (&this->child_vector, i, xml_child) {
        xml_token_t *xml_token = xml_tree_get_token(xml_child, 0);
        if (strcmp(xml_token->svalue, tag) == 0)
            return xml_child;
    }
    return NULL;
}

xml_token_t *xml_tree_find_value(xml_tree_t *this, char *tag)
{
    xml_tree_t *xml_child = xml_tree_find_child(this, tag);
    xml_token_t *xml_token = xml_tree_get_token(xml_child, 1); 
    return xml_token;
}

int xml_tree_find_ivalue(xml_tree_t *this, char *tag)
{
    xml_tree_t *xml_child = xml_tree_find_child(this, tag);
    xml_token_t *xml_token = xml_tree_get_token(xml_child, 1); 
    assert(xml_token->type == XML_TOKEN_DOUBLE);
    return (int) xml_token->dvalue;
}

char *xml_tree_find_svalue(xml_tree_t *this, char *tag)
{
    xml_tree_t *xml_child = xml_tree_find_child(this, tag);
    xml_token_t *xml_token = xml_tree_get_token(xml_child, 1); 
    assert(xml_token->type == XML_TOKEN_SYMBOL);
    return xml_token->svalue;
}

void xml_tree_dump(xml_tree_t *this, xml_file_t *xml_file)
{
    int i;
    xml_token_t *xml_token;
    xml_tree_t *xml_child;

    xml_file_indent(xml_file);
    vector_each (&this->token_vector, i, xml_token) {
        switch (xml_token->type) {
            case XML_TOKEN_STRING:
                xml_file_printf(xml_file, "\'%s\' ", xml_token->svalue);
                break;

            case XML_TOKEN_SYMBOL:
                xml_file_printf(xml_file, "%s ", xml_token->svalue);
                break;

            case XML_TOKEN_DOUBLE:
                xml_file_printf(xml_file, "%f ", xml_token->dvalue);
                break;

            default:
                xml_file_printf(xml_file, "token ");
        }
    }
    xml_file_printf(xml_file, "\n");

    vector_each (&this->child_vector, i, xml_child) {
        xml_file_push(xml_file);
        xml_tree_dump(xml_child, xml_file);
        xml_file_pop(xml_file);
    }
}

xml_tree_t *xml_tree_parse(xml_lex_t *xml_lex)
{
    xml_tree_t *xml_root = xml_tree_new();
    while (true) {
        xml_token_t *xml_token = xml_lex_get_token(xml_lex);
        if (xml_token == NULL || xml_token->type == XML_TOKEN_NEWLINE)
            break;
        xml_tree_add_token(xml_root, xml_token);
    }
    
    xml_token_t *xml_token = xml_lex_get_token(xml_lex); 
    if (xml_token->type != XML_TOKEN_PUSH) {
        xml_lex_unget_token(xml_lex, xml_token);
        return xml_root;
    }

    /* token == XML_TOKEN_PUSH */
    while (true) {
        xml_tree_t *xml_child = xml_tree_parse(xml_lex);
        xml_tree_add_child(xml_root, xml_child);

        xml_token = xml_lex_get_token(xml_lex);
        if (xml_token->type == XML_TOKEN_POP) 
            return xml_root;
        xml_lex_unget_token(xml_lex, xml_token);
    }
    return xml_root;
}
