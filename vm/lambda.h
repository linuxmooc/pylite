#ifndef _PY_LAMBDA_H
#define _PY_LAMBDA_H

#include "object.h"
#include "module.h"
#include "class.h"

typedef struct {
    int opcode;
    int size;
    xml_token_t *operand;
} insn_t;

typedef struct {
    int start;
    int end;
    int handler;
} guard_t;

typedef struct py_lambda {
    py_object_member_list;
    py_module_t *py_module;
    char *name;
    int param_count;
    int local_count;
    vector_template(int, label_vector);
    vector_template(insn_t, insn_vector);
    vector_template(char, code_vector);
    vector_template(py_object_t *, const_vector);
    vector_template(guard_t, guard_vector);
    vector_template(short, line_number_map);
} py_lambda_t;

extern py_class_t *py_lambda_class;
extern void py_lambda_class_init();
extern py_lambda_t *py_lambda_load(py_module_t *py_module, 
                                   xml_tree_t *xml_root);
extern void py_lambda_dump(py_lambda_t *this, xml_file_t *xml_file);
extern guard_t *py_lambda_locate_guard(py_lambda_t *this, int pc);

#endif
