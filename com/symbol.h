#ifndef _COM_SYMBOL_H
#define _COM_SYMBOL_H

#include "ast.h"

typedef struct {
    chain_t chain;
    int type;
    char *name; 
    int offset;
} symbol_t;

#define to_symbol(chain) ((symbol_t *)(chain))

extern symbol_t *symbol_new(int type, char *name);
extern void symbol_dump(symbol_t *this, xml_file_t *xml_file);

#endif
