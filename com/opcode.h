#ifndef _COM_OPCODE_H
#define _COM_OPCODE_H

enum {
/* 00 */  OP_NOP,
/* 01 */  OP_PRINT,
/* 02 */  OP_HALT,
/* 03 */  OP_LABEL,
/* 04 */  OP_NOT,
/* 05 */  OP_NEG,
/* 06 */  OP_COM,
/* 07 */  OP_AND,
/* 08 */  OP_OR,
/* 09 */  OP_XOR,
/* 10 */  OP_LSH,
/* 11 */  OP_RSH,
/* 12 */  OP_ADD,
/* 13 */  OP_SUB,
/* 14 */  OP_MUL,
/* 15 */  OP_DIV,
/* 16 */  OP_MOD,
/* 17 */  OP_JMP,
/* 18 */  OP_JTRUE,
/* 19 */  OP_JFALSE,
/* 20 */  OP_JLT,
/* 21 */  OP_JLE,
/* 22 */  OP_JEQ,
/* 23 */  OP_JNE,
/* 24 */  OP_JGE,
/* 25 */  OP_JGT,
/* 26 */  OP_ITERATOR,
/* 27 */  OP_NEXT,
/* 28 */  OP_CALL,
/* 29 */  OP_RETURN,
/* 30 */  OP_DUP_TOP,
/* 31 */  OP_POP,
/* 32 */  OP_LOAD_CONST,
/* 33 */  OP_LOAD_GLOBAL,
/* 34 */  OP_STORE_GLOBAL,
/* 35 */  OP_LOAD_LOCAL,
/* 36 */  OP_STORE_LOCAL,
/* 37 */  OP_LOAD_ITEM,
/* 38 */  OP_STORE_ITEM,
/* 39 */  OP_LOAD_FIELD,
/* 40 */  OP_STORE_FIELD,
/* 41 */  OP_NEW_CLASS,
/* 42 */  OP_NEW_LIST,
/* 43 */  OP_NEW_DICT,
/* 44 */  OP_NEW_TUPLE,
/* 45 */  OP_NEW_SLICE,
/* 46 */  OP_LOAD_MODULE,
/* 47 */  OP_LOAD_THIS_MODULE,
/* 48 */  OP_INHERIT,
/* 49 */  OP_RAISE,
/* 50 */  OP_CATCH,
/* 51 */  OP_FINALLY_CALL,
/* 52 */  OP_FINALLY_EXIT,
/* 53 */  OP_COUNT,
};

typedef struct {
    int id;
    char *name;
    int size;
} opcode_t;

extern opcode_t opcode_array[];
extern opcode_t *lookup_opcode(char *name);
#endif
