#include "root.h"
#include "error.h"
#include "cast.h"

py_error_t *py_error;
py_class_t *py_exception;
py_class_t *py_type_error;
py_class_t *py_index_error;
py_class_t *py_key_error;
py_class_t *py_attr_error;
py_class_t *py_name_error;
py_class_t *py_zero_division_error;
py_class_t *py_stop_iteration;
py_class_t *py_runtime_error;
py_class_t *py_io_error;

py_error_t *py_error_new(py_class_t *py_class)
{
    py_error_t *this = py_object_alloc(sizeof(py_error_t), py_class);
    stack_init(&this->location_stack);
    return this;
}

void py_error_free(py_error_t *this)
{
    int i;
    location_t location;

    vector_each (&this->location_stack, i, location) {
        free(location.file_name);
        free(location.lambda_name);
    }
    stack_destroy(&this->location_stack);
}

py_string_t *py_error_to_string(py_error_t *this)
{
    text_t text;
    text_init(&text);
    text_printf(&text, "%s:\n", this->py_class->name);

    int i;
    location_t *location;
    vector_each_address (&this->location_stack, i, location) {
        char *file_name = location->file_name;
        int line_number = location->line_number;
        char *lambda_name = location->lambda_name;
        char *format = "  At %s.py, line %d, in %s()\n";
        text_printf(&text, format, file_name, line_number, lambda_name);
    }

    py_string_t *py_result = py_string_new(text.data);
    text_destroy(&text);
    return py_result;
}

py_object_t *py_error_str(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_error_t *this = $(argv[0]);
    py_string_t *py_result = py_error_to_string(this);
    return $(py_result);
}

void py_error_dump(py_error_t *this)
{
    py_string_t *py_string = py_error_to_string(this);
    fprintf(stderr, "%s", py_string->value);
}

void py_error_add(py_error_t *this, char *file_name, 
                  char *lambda_name, int line_number)
{
    location_t location;

    location.file_name = strdup(file_name);
    location.lambda_name = strdup(lambda_name);
    location.line_number = line_number;
    stack_push(&this->location_stack, location);
}

py_object_t *py_error_alloc(py_class_t *py_class)
{
    py_error_t *this = py_object_alloc(sizeof(py_error_t), py_class);
    stack_init(&this->location_stack);
    return $(this);
}

native_t py_error_natives[] = {
    {"__str__", py_error_str},
    {NULL}
};
 
py_class_t *build_error(char *name)
{
    py_class_t *py_class = py_class_new(name);
    py_class->py_alloc = py_error_alloc;
    py_class->gc_free = $(py_error_free);
    py_class->parent= py_exception;
    py_class_register_natives(py_class, py_error_natives);
    return py_class;
}

void py_errors_init(void)
{
    py_exception = build_error("Exception");
    py_exception->parent = NULL;

    py_type_error = build_error("TypeError");
    py_index_error = build_error("IndexError");
    py_key_error = build_error("KeyError");
    py_attr_error = build_error("AttributeError");
    py_name_error = build_error("NameError");
    py_zero_division_error = build_error("ZeroDivisionError");
    py_stop_iteration = build_error("StopIteration");
    py_runtime_error = build_error("RuntimeError");
    py_io_error = build_error("IOError");
}
