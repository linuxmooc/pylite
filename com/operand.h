#ifndef _COM_OPERAND_H
#define _COM_OPERAND_H

struct lambda;

enum {
    OPERAND_STACK = 123,
    OPERAND_IMMED,

    // Constant
    OPERAND_DOUBLE,
    OPERAND_STRING,
    OPERAND_SYMBOL,
    OPERAND_NONE,
    OPERAND_TRUE,
    OPERAND_FALSE,

    // Memory
    OPERAND_GLOBAL,
    OPERAND_LOCAL,
    OPERAND_FIELD,
    OPERAND_ITEM,

    // Special
    OPERAND_LAMBDA,
    OPERAND_LABEL
};

typedef struct operand {
    int type;
    union {
        int ivalue;
        double dvalue;
        char *svalue;
    };
} operand_t;

extern operand_t *operand_immed_new(int immed);
extern operand_t *operand_double_new(double dvalue);
extern operand_t *operand_string_new(char *svalue);
extern operand_t *operand_symbol_new(char *name);
extern operand_t *operand_global_new(char *name);
extern operand_t *operand_local_new(int offset);
extern operand_t *operand_field_new(char *name);
extern operand_t *operand_item_new(void);
extern operand_t *operand_label_new(char *name);
extern bool operand_is_memory(operand_t *this);

extern operand_t *operand_stack;
extern operand_t *operand_none;
extern operand_t *operand_true;
extern operand_t *operand_false;
extern void operand_consts_init(void);

#endif
