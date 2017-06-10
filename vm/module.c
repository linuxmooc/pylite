#include "root.h"
#include "compile.h"
#include "module.h"
#include "mod.h"
#include "mod_builtin.h"
#include "gc.h"

vector_template(py_module_t *, py_module_vector);

void py_module_vector_init()
{
    vector_init(&py_module_vector);
}

py_module_t *lookup_py_module(char *name)
{
    int i;
    py_module_t *py_module;
    
    vector_each(&py_module_vector, i, py_module) {
        if (strcmp(py_module->name, name) == 0)
            return py_module;
    }
    return NULL;
}

void register_py_module(py_module_t *py_module)
{
    vector_push_back(&py_module_vector, py_module);
    gc_add_root($(py_module));
}

py_module_t *py_module_new(char *dir, char *name)
{
    py_module_t *this = py_object_alloc(sizeof(py_module_t), NULL);
    if (dir == NULL)
        this->dir = NULL;
    else
        this->dir = strdup(dir);
    this->name = strdup(name);
    return this;
}

py_module_t *load_py_module(char *dir, char *name)
{
    py_module_t *py_module = lookup_py_module(name);
    if (py_module)
        return py_module;

    char py_path[128];
    char xml_path[128];
    sprintf(py_path, "%s/%s.py", dir, name);
    sprintf(xml_path, "%s/%s.xml", dir, name);
    compile_python(py_path, xml_path);

    py_module = py_module_new(dir, name);
    register_py_module(py_module);

    xml_lex_t *xml_lex = xml_lex_new(xml_path);
    xml_tree_t *xml_root = xml_tree_parse(xml_lex);

    py_lambda_t *py_lambda = py_lambda_load(py_module, xml_root);
    py_object_t *py_result = vm_call_lambda(py_lambda, 0, vm_stack + sp);
    if (py_result == NULL)
        return NULL;
    return py_module;
}

static void invalid_suffix()
{
    fprintf(stderr, "Suffix must be '.py' or '.xml'\n");
    exit(EXIT_FAILURE);
}

static void check_suffix(char *path, char *dir, char *name)
{
    char *suffix = rindex(path, '.');
    if (suffix == NULL)
        invalid_suffix();

    char *slash = rindex(path, '/');
    if (slash == NULL) {
        strcpy(dir, ".");

        memcpy(name, path, suffix - path);
        name[suffix - path] = 0;
    } else {
        memcpy(dir, path, slash - path);
        dir[slash - path] = 0;

        char *p = slash + 1;
        memcpy(name, p, suffix - p);
        name[suffix - p] = 0;
    }

    if (strcmp(suffix, ".py") == 0)
        return;

    if (strcmp(suffix, ".xml") == 0)
        return;

    invalid_suffix();
}

void check_suffix_test(char *path)
{
    char dir[128];
    char name[128];

    check_suffix(path, dir, name);
    printf("%-16s%-16s%-16s\n", path, dir, name);
}

void open_main_module(char *path)
{
    char dir[128];
    char name[128];
    check_suffix(path, dir, name);

#if 0
    // path             dir     name
    // '/foo/math.py'   '/foo'      'math'
    // 'foo/math.py'    'foo'       'math'
    // '../math.py'     '..'        'math'
    // 'math.py'        '.'         'math'
    check_suffix_test("/foo/math.py");
    check_suffix_test("foo/math.py");
    check_suffix_test("../math.py");
    check_suffix_test("math.py");
    exit(EXIT_SUCCESS);
#endif

    py_module_t *py_module = load_py_module(dir, name);
    if (py_module == NULL) {
        fflush(stdout);
        py_error_dump(py_error);
        py_error = NULL;
    }
    assert(sp == VM_STACK_DEPTH);
}

void py_module_import_object(py_module_t *this, char *name, 
                             py_object_t *py_value)
{
    py_symbol_t *py_name = py_symbol_new(name);
    py_object_set_field($(this), py_name, py_value);
}

void py_module_import_double(py_module_t *this, char *name, double value)
{
    py_symbol_t *py_name = py_symbol_new(name);
    py_double_t *py_value = py_double_new(value);
    py_object_set_field($(this), py_name, $(py_value));
}

void py_module_import_string(py_module_t *this, char *name, char *value)
{
    py_symbol_t *py_name = py_symbol_new(name);
    py_string_t *py_value = py_string_new(value);
    py_object_set_field($(this), py_name, $(py_value));
}

void py_module_import_lambda(py_module_t *this, char *name, handler_t value)
{
    py_symbol_t *py_name = py_symbol_new(name);
    py_native_t *py_value = py_native_new(py_name, value);
    py_object_set_field($(this), py_name, $(py_value));
}

void py_module_import_class(py_module_t *this, char *name, py_class_t *value)
{
    py_symbol_t *py_name = py_symbol_new(name);
    py_object_t *py_value = $(value);
    py_object_set_field($(this), py_name, py_value);
}

void py_module_import_all(py_module_t *this, py_module_t *that)
{
    int i;
    field_t field;
    vector_each (&that->field_vector, i, field)
        vector_push_back(&this->field_vector, field);
}
