#!/usr/bin/python
ast_list = """
module
clause_list
clause

express_clause
assign_clause
del_clause
pass_clause
import_clause
from_clause
global_clause
print_clause
assert_clause
break_clause
continue_clause
return_clause

group_clause
if_clause
elif_clause
else_clause
while_clause
for_clause
try_clause
except_clause
finally_clause
raise_clause
def_clause
param_list
class_clause

comma_express
lambda_express
slice_express
binary_express
unary_express
call_express
arg_list
id_list
item_express
field_express
postfix_express
token
list_express
dict_express
dict_item
tuple_express
"""

ast_list = filter(lambda x: x != "", ast_list.split("\n"))

def print_enum(ast_list):
    print "enum {"
    index = 0
    for ast in ast_list:
        print "    /* %02d */  AST_%s," % (index, ast.upper())
        index += 1
    print "              AST_NUMBER"
    print "};"

def print_names(ast_list):
    print "char *ast_name_array[] = {"
    index = 0
    for ast in ast_list:
        print "    /* %02d */  \"%s\"," % (index, ast)
        index += 1
    print "};"

def print_compile(ast_list):
    for ast in ast_list:
        print "operand_t *compile_%s(ast_t *ast, lambda_t *lambda)" % ast
        print "{"
        print "    return NULL;"
        print "}"
        print

def print_map(ast_list):
    for ast in ast_list:
        print "        map(%s, %s);" % (ast.upper(), ast)

print_enum(ast_list)
print_names(ast_list)
print_compile(ast_list)
print_map(ast_list)
