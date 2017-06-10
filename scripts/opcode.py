#!/usr/bin/python
import re

file = open("opcode.map")
record_list = []

def parse_record():
    for line in file:
        if line == "\n":
            continue
        line = line[0:-1]
        record = re.findall(r'[^ \t]+', line)
        record_list.append(record) 
    return record_list

def print_enum(record_list):
    print "enum {"
    index = 0
    for record in record_list:
        print "/* %02d */  OP_%s," % (index, record[0].upper())
        index += 1
    print "/* %02d */  OP_COUNT," % index
    print "};\n"

def print_type():
    print "typedef struct {"
    print "    int id;"
    print "    char *name;"
    print "    int size;"
    print "} opcode_t;"

def print_info(record_list):
    print "opcode_t opcode_array[] = {"
    index = 0
    for record in record_list:
        name = record[0]
        size = record[1]
        print '    {%2d, "%s", %s},' % (index, name, size)
        index += 1
    print "};"

def print_jump_table(record_list):
    print "static void *jump_table[] = {"
    index = 0
    for record in record_list:
        print "    /* %02d */ &&label_OP_%s," % (index, record[0].upper())
        index += 1
    print "};"

def print_dispatch(index, record):
    opcode = record[0].upper()
    insn = record[0]
    print "    /* %02d */ dispatch(OP_%s, insn_%s);" % (index, opcode, insn)

def print_dispatch_table(record_list):
    index = 0
    for record in record_list:
        print_dispatch(index, record)
        index += 1

def print_insn(record):
    name = record[0]
    size = record[1]
    print "interpret(OP_%s)" % name.upper()
    print "{"
    print "    pc += %s;" % size
    print "    break;"
    print "}"
    print

def print_insn_list(record_list):
    for record in record_list:
        print_insn(record)

record_list = parse_record()
print_enum(record_list)
print_type()
print_info(record_list)

#print_jump_table(record_list)
#print_insn_list(record_list)
