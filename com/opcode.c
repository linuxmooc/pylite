#include "root.h"
#include "opcode.h"

opcode_t opcode_array[] = {
    { 0, "nop", 1},
    { 1, "print", 1},
    { 2, "halt", 1},
    { 3, "label", 0},
    { 4, "not", 1},
    { 5, "neg", 1},
    { 6, "com", 1},
    { 7, "and", 1},
    { 8, "or", 1},
    { 9, "xor", 1},
    {10, "lsh", 1},
    {11, "rsh", 1},
    {12, "add", 1},
    {13, "sub", 1},
    {14, "mul", 1},
    {15, "div", 1},
    {16, "mod", 1},
    {17, "jmp", 3},
    {18, "jtrue", 3},
    {19, "jfalse", 3},
    {20, "jlt", 3},
    {21, "jle", 3},
    {22, "jeq", 3},
    {23, "jne", 3},
    {24, "jge", 3},
    {25, "jgt", 3},
    {26, "iterator", 1},
    {27, "next", 3},
    {28, "call", 3},
    {29, "return", 1},
    {30, "dup_top", 3},
    {31, "pop", 1},
    {32, "load_const", 3},
    {33, "load_global", 3},
    {34, "store_global", 3},
    {35, "load_local", 3},
    {36, "store_local", 3},
    {37, "load_item", 1},
    {38, "store_item", 1},
    {39, "load_field", 3},
    {40, "store_field", 3},
    {41, "new_class", 3},
    {42, "new_list", 3},
    {43, "new_dict", 1},
    {44, "new_tuple", 3},
    {45, "new_slice", 1},
    {46, "load_module", 3},
    {47, "load_this_module", 1},
    {48, "inherit", 1},
    {49, "raise", 1},
    {50, "catch", 1},
    {51, "finally_call", 3},
    {52, "finally_exit", 3},
};

opcode_t *lookup_opcode(char *name)
{
    for (int i = 0; i < OP_COUNT; i++) {
        opcode_t *opcode = opcode_array + i;
        if (strcmp(opcode->name, name) == 0) 
            return opcode;
    }
    return NULL;
}
