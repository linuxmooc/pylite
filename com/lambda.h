#ifndef _COM_LAMBDA_H
#define _COM_LAMBDA_H

#include "insn.h"

typedef struct {
    operand_t *start;
    operand_t *end;
    operand_t *handler;
} guard_t;

typedef struct lambda {
    int type;
    char *name;

    int param_count;
    int local_count;
    list_t insn_list; 

    stack_template(operand_t *, break_stack);
    stack_template(operand_t *, continue_stack);
    vector_template(operand_t *, const_vector);
    stack_template(operand_t *, finally_stack);

    vector_template(guard_t, guard_vector);
    stack_template(int, line_number_stack);
    vector_template(int, line_number_map);
    int label_count;
} lambda_t;

extern lambda_t *lambda_new(char *name);
extern void lambda_output(lambda_t *this, xml_file_t *xml_file);
extern void lambda_emit_insn(lambda_t *this, int opcode, char *format, ...);

extern int lambda_new_local(lambda_t *this);
extern void lambda_emit_label(lambda_t *this, operand_t *label);
extern operand_t *lambda_new_label(lambda_t *this);
extern int lambda_add_const(lambda_t *this, operand_t *operand);

#endif
