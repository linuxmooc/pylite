#ifndef _XML_TREE_H
#define _XML_TREE_H

#include "vector.h"
#include "xml_file.h"
#include "xml_lex.h"

typedef struct xml_tree {
    int type;
    vector_template(xml_token_t *, token_vector);
    vector_template(struct xml_tree *, child_vector);
} xml_tree_t;

extern xml_tree_t *xml_tree_new();
extern xml_tree_t *xml_tree_parse(xml_lex_t *xml_lex);

extern xml_tree_t *xml_tree_find_child(xml_tree_t *this, char *tag);
extern xml_token_t *xml_tree_find_value(xml_tree_t *this, char *tag);
extern int xml_tree_find_ivalue(xml_tree_t *this, char *tag);
extern char *xml_tree_find_svalue(xml_tree_t *this, char *tag);

extern xml_tree_t *xml_tree_get_child(xml_tree_t *this, int index);
extern xml_token_t *xml_tree_get_token(xml_tree_t *this, int index);
extern char *xml_tree_get_tag(xml_tree_t *this);
extern void xml_tree_dump(xml_tree_t *this, xml_file_t *xml_file);

#endif
