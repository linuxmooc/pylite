#include "root.h"
#include "lambda.h"
#include "opcode.h"
#include "double.h"
#include "symbol.h"
#include "string.h"
#include "bool.h"
#include "none.h"
#include "gc.h"
#include "asm.h"

void py_lambda_build_label_vector(py_lambda_t *this, xml_tree_t *xml_root)
{
    int label_count = xml_tree_find_ivalue(xml_root, "label_count");
    vector_init(&this->label_vector);
    for (int j = 0; j < label_count; j++)
        vector_push_back(&this->label_vector, 0);

    int pc = 0;
    int i;
    insn_t insn;
    vector_each (&this->insn_vector, i, insn) {
        if (insn.opcode == OP_LABEL) {
            xml_token_t *xml_operand = insn.operand;
            assert(xml_operand->svalue[0] == 'L');
            int operand = atoi(xml_operand->svalue + 1);
            vector_set(&this->label_vector, operand, pc);
        }
        pc += insn.size;
    }
}

void py_lambda_add_code(py_lambda_t *this, char code)
{
    vector_push_back(&this->code_vector, code);
}

void py_lambda_build_code(py_lambda_t *this, insn_t *insn)
{
    int opcode = insn->opcode;
    py_lambda_add_code(this, opcode);
    if (insn->operand == NULL)
        return;
    int operand = py_lambda_build_operand(this, insn->operand);
    py_lambda_add_code(this, operand & 0xFF);
    py_lambda_add_code(this, (operand >> 8) & 0xFF);
}

void py_lambda_build_code_vector(py_lambda_t *this)
{
    vector_init(&this->code_vector);

    int pc = 0;
    int i;
    insn_t insn;
    vector_each (&this->insn_vector, i, insn) {
        if (insn.opcode == OP_LABEL)
            continue;
        py_lambda_build_code(this, &insn);
        pc += insn.size;
    }
}

int py_lambda_build_operand(py_lambda_t *this, xml_token_t *xml_operand)
{
    if (xml_operand->type == XML_TOKEN_DOUBLE)
        return xml_operand->dvalue;

    assert(xml_operand->type == XML_TOKEN_SYMBOL);
    char prefix = xml_operand->svalue[0];
    assert(prefix == 'L' || prefix == 'c');

    int value = atoi(xml_operand->svalue + 1);
    if (prefix == 'L')
        value = vector_get(&this->label_vector, value);
    return value;
}

void py_lambda_build_insn(py_lambda_t *this, xml_tree_t *xml_insn)
{
    xml_token_t *xml_opcode = xml_tree_get_token(xml_insn, 0);
    opcode_t *opcode = lookup_opcode(xml_opcode->svalue);

    insn_t insn;
    insn.opcode = opcode->id;
    insn.size = opcode->size;
    if (xml_insn->token_vector.count == 2) {
        insn.operand = xml_tree_get_token(xml_insn, 1);
    } else {
        insn.operand = NULL;
    }
    vector_push_back(&this->insn_vector, insn);
}

void py_lambda_build_insn_vector(py_lambda_t *this, xml_tree_t *xml_root)
{
    vector_init(&this->insn_vector);
    xml_tree_t *xml_insn_vector;
    xml_insn_vector = xml_tree_find_child(xml_root, "insn_vector");

    int i;
    xml_tree_t *xml_insn;
    vector_each (&xml_insn_vector->child_vector, i, xml_insn) {
        py_lambda_build_insn(this, xml_insn);
    }
}

void py_lambda_assemble(py_lambda_t *this, xml_tree_t *xml_root)
{
    py_lambda_build_insn_vector(this, xml_root);
    py_lambda_build_label_vector(this, xml_root);
    py_lambda_build_code_vector(this);
}
