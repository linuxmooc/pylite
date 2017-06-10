#include "root.h"
#include "insn.h"

insn_t *insn_new(int opcode)
{
    insn_t *this = malloc(sizeof(insn_t));
    chain_init(&this->chain);
    this->opcode = opcode_array + opcode;
    this->operand_count = 0;
    for (int i = 0; i < MAX_OPERAND; i++)
        this->operand_array[i] = NULL;
    return this;
}

void insn_delete(insn_t *this)
{
    free(this);
}

void insn_add_operand(insn_t *this, operand_t *operand)
{
    this->operand_array[this->operand_count] = operand;
    this->operand_count += 1;
}

operand_t *insn_get_operand(insn_t *this, int index)
{
    assert(index < MAX_OPERAND);
    return this->operand_array[index];
}

/** 
 * %i   immed
 * %d   integer const 
 * %f   float const 
 * %s   string const
 * %S   symbol
 * %o   operand
 **/
void insn_format(insn_t *this, int opcode, char *format, va_list ap)
{
    if (format == NULL)
        return;

    assert(format[0] == '%');
    char flag = format[1];
    operand_t *operand = NULL;

    switch (flag) {
        case 'i':
            operand = operand_immed_new(va_arg(ap, int));
            break;

        case 'd':
            operand = operand_double_new(va_arg(ap, int));
            break;

        case 'f':
            operand = operand_double_new(va_arg(ap, double));
            break;

        case 's':
            operand = operand_string_new(va_arg(ap, char *));
            break;

        case 'S':
            operand = operand_symbol_new(va_arg(ap, char *));
            break;

        case 'o':
            operand = va_arg(ap, operand_t *);
            break;

        default:
            assert(0);
    }

    insn_add_operand(this, operand);
}
