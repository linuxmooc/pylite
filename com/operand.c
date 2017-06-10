#include "root.h"
#include "operand.h"
#include "lambda.h"

operand_t *operand_none;
operand_t *operand_true;
operand_t *operand_false;
operand_t *operand_stack;

operand_t *operand_new(int type)
{
    operand_t *this = malloc(sizeof(operand_t));
    this->type = type;
    return this;
}

void operand_consts_init(void)
{
    operand_none = operand_new(OPERAND_NONE);
    operand_true = operand_new(OPERAND_TRUE);
    operand_false = operand_new(OPERAND_FALSE);
    operand_stack = operand_new(OPERAND_STACK);
}

operand_t *operand_immed_new(int immed)
{
    operand_t *this = operand_new(OPERAND_IMMED);
    this->ivalue = immed;
    return this;
}

operand_t *operand_double_new(double dvalue)
{
    operand_t *this = operand_new(OPERAND_DOUBLE);
    this->dvalue = dvalue;
    return this;
}

operand_t *operand_string_new(char *svalue)
{
    operand_t *this = operand_new(OPERAND_STRING);
    this->svalue = strdup(svalue);
    return this;
}

operand_t *operand_symbol_new(char *name)
{
    operand_t *this = operand_new(OPERAND_SYMBOL);
    this->svalue = strdup(name);
    return this;
}

operand_t *operand_global_new(char *name)
{
    operand_t *this = operand_new(OPERAND_GLOBAL);
    this->svalue = strdup(name);
    return this;
}

operand_t *operand_local_new(int offset)
{
    operand_t *this = operand_new(OPERAND_LOCAL);
    this->ivalue = offset;
    return this;
}

operand_t *operand_item_new(void)
{
    operand_t *this = operand_new(OPERAND_ITEM);
    return this;
}

operand_t *operand_field_new(char *name)
{
    operand_t *this = operand_new(OPERAND_FIELD);
    this->svalue = strdup(name);
    return this;
}

operand_t *operand_label_new(char *name)
{
    operand_t *this = operand_new(OPERAND_LABEL);
    this->svalue = strdup(name);
    return this;
}

bool operand_is_memory(operand_t *this)
{
    switch (this->type) {
        case OPERAND_GLOBAL:
        case OPERAND_LOCAL:
        case OPERAND_ITEM:
        case OPERAND_FIELD:
            return true;
    }
    return false;
}


