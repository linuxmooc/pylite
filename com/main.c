#include "root.h"
#include "ast.h"
#include "lex.h"
#include "lambda.h"
#include "compile.h"

int flag_dump_lex;
int flag_dump_ast;

static char *usage_text =
"Usage: pycom [options] file.py\n"
"General options:\n"
"  -help          display this message\n"
"  -o file.xml    output to file.xml\n"
"Dump options:\n"
"  -dump-lex      display lex\n"
"  -dump-ast      display abstract syntax tree\n"
"  -dump-symbol   display symbol\n"
;

static void usage(void)
{
    printf("pycom, version %s\n", "0.1");
    printf("%s", usage_text);
    exit(EXIT_FAILURE);
}

extern int yydebug;
extern int yyparse(lex_t *lex, ast_t **ast);

ast_t *front_process(char *file_name)
{
    lex_t *lex = lex_new(file_name);
    if (flag_dump_lex) {
        lex_dump(lex);
        exit(EXIT_SUCCESS);
    }

    ast_t *ast = NULL;
    yydebug = 0;
    yyparse(lex, &ast);
    ast_locate(ast, file_name);
    if (flag_dump_ast) {
        ast_dump(ast, xml_stdout);
        exit(EXIT_SUCCESS);
    }
    return ast;
}

void compute_output_path(char *input_path, char *output_path)
{
    int len = strlen(input_path);
    char *suffix = input_path + len - 3;
    if (strcmp(suffix, ".py") != 0)
        usage();

    memcpy(output_path, input_path, len - 3);
    strcpy(output_path + len - 3, ".xml");
}

#define is_flag(flag) strcmp(argv[i], flag) == 0

int main(int argc, char *argv[])
{
    if (argc == 1)
        usage();

    char *input_path = NULL;
    char *output_path = "/dev/stdout";
    for (int i = 1; i < argc; i++) {
        if (is_flag("-dump-lex")) {
            flag_dump_lex = 1;
            continue;
        }

        if (is_flag("-dump-ast")) {
            flag_dump_ast = 1;
            continue;
        }

        if (is_flag("-o")) {
            i++;
            output_path = argv[i];
            continue;
        }

        input_path = argv[i];
    }

    xml_stdout = xml_file_new("/dev/stdout");
    operand_consts_init();
    ast_t *module = front_process(input_path);
    lambda_t *lambda = compile_module(module);

    xml_file_t *xml_output = xml_file_new(output_path);
    lambda_output(lambda, xml_output);
    xml_file_delete(xml_stdout);
    return 0;
}
