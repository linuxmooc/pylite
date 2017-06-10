%{
#include "root.h"
#include "lex.h"
#include "ast.h"
#define YYSTYPE ast_t *

extern int yylex();
void yyerror(lex_t *lex, ast_t **module, char *message);
%}
%pure-parser
%parse-param { lex_t *lex          }
%parse-param { ast_t **module      }
%lex-param   { lex_t *lex          }

%token TOKEN_BEGIN TOKEN_END
%token TOKEN_GLOBAL TOKEN_FROM TOKEN_IMPORT TOKEN_DEL TOKEN_PASS 
%token TOKEN_PRINT TOKEN_ASSERT
%token TOKEN_IF TOKEN_ELIF TOKEN_ELSE
%token TOKEN_FOR TOKEN_WHILE TOKEN_IN
%token TOKEN_CONTINUE TOKEN_BREAK TOKEN_RETURN 
%token TOKEN_TRY TOKEN_EXCEPT TOKEN_AS TOKEN_FINALLY TOKEN_RAISE
%token TOKEN_CLASS TOKEN_DEF TOKEN_LAMBDA
%token TOKEN_DOUBLE_CONST TOKEN_STRING_CONST 
%token TOKEN_ID TOKEN_NONE TOKEN_TRUE TOKEN_FALSE 
%token TOKEN_MUL_ASSIGN TOKEN_DIV_ASSIGN TOKEN_MOD_ASSIGN 
%token TOKEN_ADD_ASSIGN TOKEN_SUB_ASSIGN
%token TOKEN_LSH_ASSIGN TOKEN_RSH_ASSIGN
%token TOKEN_AND_ASSIGN TOKEN_XOR_ASSIGN TOKEN_OR_ASSIGN 

%left TOKEN_OR
%left TOKEN_AND
%left TOKEN_IS
%left '|'
%left '^'
%left '&'
%left TOKEN_EQ TOKEN_NE
%left '<' TOKEN_LE '>' TOKEN_GE
%left TOKEN_LSH TOKEN_RSH
%left TOKEN_NOT '+' '-'
%left '*' '/' '%'
%nonassoc '~'

%%
start
    : clause_list
        { $$ = *module = $1;                                           }
    ;

clause_list
    :
        { $$ = ast_new(AST_CLAUSE_LIST, 0);                            }
    | clause_list '\n'
    | clause_list clause 
        { $$ = ast_add_child($1, $2);                                  }
    ; 

clause
    : simple_clause
    | compound_clause
    ;

simple_clause
    : print_clause
    | express_clause
    | assign_clause
    | del_clause
    | pass_clause
    | import_clause
    | from_clause
    | global_clause
    | assert_clause
    | break_clause
    | continue_clause
    | return_clause
    ;

print_clause
    : TOKEN_PRINT '\n'
        { $$ = ast_new(AST_PRINT_CLAUSE, 1, NULL);                     }
    | TOKEN_PRINT express '\n'
        { $$ = ast_new(AST_PRINT_CLAUSE, 1, $2);                       }
    ;

express_clause
    : express '\n'
        { $$ = ast_new(AST_EXPRESS_CLAUSE, 1, $1);                     }
    ;

assign_clause
    : express assign_operator express '\n'
        { $$ = ast_new(AST_ASSIGN_CLAUSE, 3, $1, $2, $3);              }
    ;

assign_operator 
    : '=' 
    | TOKEN_MUL_ASSIGN
    | TOKEN_DIV_ASSIGN
    | TOKEN_MOD_ASSIGN
    | TOKEN_ADD_ASSIGN
    | TOKEN_SUB_ASSIGN
    | TOKEN_LSH_ASSIGN
    | TOKEN_RSH_ASSIGN
    | TOKEN_AND_ASSIGN
    | TOKEN_XOR_ASSIGN
    | TOKEN_OR_ASSIGN
    ; 

del_clause
    : TOKEN_DEL express '\n'
        { $$ = ast_new(AST_DEL_CLAUSE, 1, $2);                         }
    ;

pass_clause
    : TOKEN_PASS '\n'
        { $$ = ast_new(AST_PASS_CLAUSE, 0);                            }
    ;

import_clause
    : TOKEN_IMPORT id_list '\n'
        { $$ = ast_new(AST_IMPORT_CLAUSE, 1, $2);                      }
    ;

from_clause
    : TOKEN_FROM TOKEN_ID TOKEN_IMPORT id_list '\n'
        { $$ = ast_new(AST_FROM_CLAUSE, 2, $2, $4);                    }
    ;

global_clause
    : TOKEN_GLOBAL id_list '\n'
        { $$ = ast_new(AST_GLOBAL_CLAUSE, 1, $2);                      }
    ;

assert_clause
    : TOKEN_ASSERT express '\n'
        { $$ = ast_new(AST_ASSERT_CLAUSE, 0);                          }
    ;

break_clause
    : TOKEN_BREAK '\n'
        { $$ = ast_new(AST_BREAK_CLAUSE, 0);                           }
    ;

continue_clause
    : TOKEN_CONTINUE '\n'
        { $$ = ast_new(AST_CONTINUE_CLAUSE, 0);                        }
    ;

return_clause
    : TOKEN_RETURN '\n'
        { $$ = ast_new(AST_RETURN_CLAUSE, 1, NULL);                    }
    | TOKEN_RETURN express '\n'
        { $$ = ast_new(AST_RETURN_CLAUSE, 1, $2);                      }
    ;

compound_clause
    : group_clause
    | if_clause
    | while_clause
    | for_clause
    | def_clause
    | class_clause
    | try_clause
    | raise_clause
    ;

group_clause
    : TOKEN_BEGIN clause_list TOKEN_END
        { $$ = $2;                                                     }
    ; 

colon
    : ':' '\n'
    ;

if_clause
    : TOKEN_IF express colon group_clause
        { $$ = ast_new(AST_IF_CLAUSE, 2, $2, $4);                      }
    | if_clause elif_clause
        { $$ = ast_add_child($1, $2);                                  }
    | if_clause else_clause
        { $$ = ast_add_child($1, $2);                                  }
    ;

elif_clause
    : TOKEN_ELIF express colon group_clause
        { $$ = ast_new(AST_ELIF_CLAUSE, 2, $2, $4);                    }
    ;

else_clause
    : TOKEN_ELSE colon group_clause
        { $$ = ast_new(AST_ELSE_CLAUSE, 1, $3);                        }
    ;

while_clause
    : TOKEN_WHILE express colon group_clause
        { $$ = ast_new(AST_WHILE_CLAUSE, 2, $2, $4);                   }
    | TOKEN_WHILE express colon group_clause else_clause
    ;

for_clause
    : TOKEN_FOR express TOKEN_IN express colon group_clause
        { $$ = ast_new(AST_FOR_CLAUSE, 3, $2, $4, $6);                 }
    ;

try_clause
    : try_body except_clause
        { $$ = ast_new(AST_TRY_CLAUSE, 3, $1, $2, NULL);               }
    | try_body finally_clause
        { $$ = ast_new(AST_TRY_CLAUSE, 3, $1, NULL, $2);               }
    | try_body except_clause finally_clause
        { $$ = ast_new(AST_TRY_CLAUSE, 3, $1, $2, $3);                 }
    ;

try_body
    : TOKEN_TRY colon group_clause
        { $$ = $3;                                                     }
    ;

except_clause
    : TOKEN_EXCEPT colon group_clause
        { $$ = ast_new(AST_EXCEPT_CLAUSE, 3, NULL, NULL, $3);          }
    | TOKEN_EXCEPT express colon group_clause
        { $$ = ast_new(AST_EXCEPT_CLAUSE, 3, $2, NULL, $4);            }
    | TOKEN_EXCEPT express TOKEN_AS TOKEN_ID colon group_clause
        { $$ = ast_new(AST_EXCEPT_CLAUSE, 3, $2, $4, $6);              }
    ;

finally_clause
    : TOKEN_FINALLY colon group_clause
        { $$ = ast_new(AST_FINALLY_CLAUSE, 1, $3);                     }
    ;

raise_clause
    : TOKEN_RAISE express '\n'
        { $$ = ast_new(AST_RAISE_CLAUSE, 1, $2);                       }
    ;

def_clause
    : TOKEN_DEF TOKEN_ID signature colon group_clause 
        { $$ = ast_new(AST_DEF_CLAUSE, 3, $2, $3, $5);                 }
    ;

signature
    : '(' ')'
        { $$ = ast_new(AST_PARAM_LIST, 0);                             }
    | '(' param_list ')'
        { $$ = $2;                                                     }
    ;

param_list
    : TOKEN_ID
        { $$ = ast_new(AST_PARAM_LIST, 1, $1);                         }
    | param_list ',' TOKEN_ID
        { $$ = ast_add_child($1, $3);                                  }
    ;

class_clause
    : TOKEN_CLASS TOKEN_ID parent colon group_clause 
        { $$ = ast_new(AST_CLASS_CLAUSE, 3, $2, $3, $5);               }
    ;

parent
    :
        { $$ = NULL;                                                   }
    | '(' ')'
        { $$ = NULL;                                                   }
    | '(' TOKEN_ID ')'
        { $$ = $2;                                                     }
    ;

express
    : binary_express
    | TOKEN_LAMBDA id_list ':' binary_express
        { $$ = ast_new(AST_LAMBDA_EXPRESS, 2, $2, $4);                 }
    ; 

binary_express
    : unary_express
    | binary_express TOKEN_OR binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express TOKEN_AND binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '|' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '^' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '&' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express TOKEN_EQ binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express TOKEN_NE binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '<' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '>' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express TOKEN_LE binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express TOKEN_GE binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express TOKEN_LSH binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express TOKEN_RSH binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '+' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '-' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '*' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '/' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    | binary_express '%' binary_express
        { $$ = ast_new(AST_BINARY_EXPRESS, 3, $1, $2, $3);             }
    ;

unary_express 
    : postfix_express
    | unary_operator unary_express                
        { $$ = ast_new(AST_UNARY_EXPRESS, 2, $1, $2);                  }
    ;

unary_operator
    : '+' 
    | '-' 
    | '~' 
    | TOKEN_NOT
    ; 

postfix_express 
    : primary_express 
    | postfix_express '(' arg_list ')'
        { $$ = ast_new(AST_CALL_EXPRESS, 2, $1, $3);                   }
    | postfix_express '[' express ']'         
        { $$ = ast_new(AST_ITEM_EXPRESS, 2, $1, $3);                   }
    | postfix_express '[' slice_express ']'         
        { $$ = ast_new(AST_ITEM_EXPRESS, 2, $1, $3);                   }
    | postfix_express '.' TOKEN_ID           
        { $$ = ast_new(AST_FIELD_EXPRESS, 2, $1, $3);                  }
    ;

slice_express
    : express ':' express
        { $$ = ast_new(AST_SLICE_EXPRESS, 2, $1, $3);                  }
    | express ':' 
        { $$ = ast_new(AST_SLICE_EXPRESS, 2, $1, NULL);                }
    ;

id_list
    :
        { $$ = ast_new(AST_ID_LIST, 0);                                }
    | id_list0
    ;

id_list0
    : TOKEN_ID
        { $$ = ast_new(AST_ID_LIST, 1, $1);                            }
    | id_list0 ',' TOKEN_ID
        { $$ = ast_add_child($1, $3);                                  }
    ; 

arg_list
    :
        { $$ = ast_new(AST_ARG_LIST, 0);                               }
    | arg_list0
        { $$ = $1;                                                     }
    ; 

arg_list0
    : express 
        { $$ = ast_new(AST_ARG_LIST, 1, $1);                           }
    | arg_list0 ',' express           
        { $$ = ast_add_child($1, $3);                                  }
    ; 

primary_express 
    : TOKEN_NONE
    | TOKEN_TRUE
    | TOKEN_FALSE
    | TOKEN_ID
    | TOKEN_DOUBLE_CONST
    | TOKEN_STRING_CONST
    | '(' express ')'
        { $$ = $2;                                                     }
    | list_express
    | dict_express
    | tuple_express
    ;

list_express
    : '[' ']'
        { $$ = ast_new(AST_LIST_EXPRESS, 0);                           }
    | '[' list_item_list ']'
        { $$ = $2;                                                     }
    ;

list_item_list
    : express
        { $$ = ast_new(AST_LIST_EXPRESS, 1, $1);                       }
    | list_item_list ',' express
        { $$ = ast_add_child($1, $3);                                  }
    ;

dict_express
    : '{' '}'
        { $$ = ast_new(AST_DICT_EXPRESS, 0);                           }
    | '{' dict_item_list '}'
        { $$ = $2;                                                     }
    ;

dict_item_list
    : dict_item
        { $$ = ast_new(AST_DICT_EXPRESS, 1, $1);                       }
    | dict_item_list ',' dict_item 
        { $$ = ast_add_child($1, $3);                                  }
    ;

dict_item
    : primary_express ':' express
        { $$ = ast_new(AST_DICT_ITEM, 2, $1, $3);                      }
    ;

tuple_express
    : '(' ')'
        { $$ = ast_new(AST_TUPLE_EXPRESS, 0);                          }
    | '(' express ',' ')'
        { $$ = ast_new(AST_TUPLE_EXPRESS, 1, $2);                      }
    | '(' express ',' tuple_item_list ')'
        { $$ = ast_prepend_child($4, $2);                              }
    ;

tuple_item_list
    : express
        { $$ = ast_new(AST_TUPLE_EXPRESS, 1, $1);                      }
    | tuple_item_list ',' express
        { $$ = ast_add_child($1, $3);                                  }
    ;
%%

int yylex(YYSTYPE *yylval, lex_t *lex)
{
    ast_t *token = lex_get_token(lex);
    *yylval = token;
    if (!token)
        return 0;
    return token->token;
}
