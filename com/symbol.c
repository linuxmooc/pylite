#include "root.h"
#include "symbol.h"
#include "scope.h"

symbol_t *symbol_new(int type, char *name)
{
    symbol_t *this = malloc(sizeof(symbol_t));
    chain_init(&this->chain);
    this->type = type;
    this->name = strdup(name);
    this->offset = 0;
    return this;
}

void symbol_dump(symbol_t *this, xml_file_t *xml_file)
{
    xml_file_dump(xml_file, "symbol");
    xml_file_push(xml_file);

    char *text = NULL;
    switch (this->type) {
        case SCOPE_GLOBAL:
            text = "SCOPE_GLOBAL";
            break;

        case SCOPE_CLASS:
            text = "SCOPE_CLASS";
            break;

        case SCOPE_LOCAL:
            text = "SCOPE_LOCAL";
            break;
    }
    xml_file_dump(xml_file, "type %s", text);
    xml_file_dump(xml_file, "name %s", this->name);
    xml_file_dump(xml_file, "offset %d", this->offset);
    xml_file_pop(xml_file);
}
