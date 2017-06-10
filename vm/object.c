#include "root.h"
#include "object.h"
#include "class.h"
#include "lambda.h"
#include "native.h"
#include "method.h"
#include "error.h"
#include "vm.h"
#include "string.h"
#include "gc.h"
#include "alloc.h"

py_class_t *py_object_class;

void *py_object_alloc(int size, struct py_class *py_class)
{
    py_object_t *this = gc_alloc_object(size); 
    this->py_class = py_class;
    vector_init(&this->field_vector);
    return this;
}

py_object_t *py_object_new(py_class_t *py_class)
{
    py_object_t *this = py_object_alloc(sizeof(py_object_t), py_class);
    return this;
}

void py_object_add_field(py_object_t *this, py_symbol_t *py_name, 
                         py_object_t *py_value)
{
    field_t field;
    field.py_name = py_name;
    field.py_value = py_value;
    vector_push_back(&this->field_vector, field); 
}

py_object_t *py_object_get_field(py_object_t *this, py_symbol_t *py_name)
{
    int i;
    field_t *field;
    vector_each_address (&this->field_vector, i, field) {
        if (field->py_name == py_name) {
            return field->py_value;
        }
    }
    return NULL;
}

py_object_t *py_object_load_field(py_object_t *this, py_symbol_t *py_name)
{
    py_object_t *py_result = py_object_get_field(this, py_name);
    if (py_result)
        return py_result;

    py_class_t *py_class = this->py_class; 
    if (py_class == NULL)
        return NULL;

    py_result = py_class_load_field(py_class, py_name);
    if (py_result == NULL)
        return NULL;

    // Create a bound method
    if (py_result->py_class == py_lambda_class || 
        py_result->py_class == py_native_class)
        return $(py_method_new(this, py_result));
    else
        return py_result;
}

void py_object_set_field(py_object_t *this, 
                         py_symbol_t *py_name, py_object_t *py_value)
{
    int i;
    field_t *field;
    vector_each_address (&this->field_vector, i, field) {
        if (field->py_name == py_name) {
            field->py_value = py_value;
            return;
        }
    }   

    py_object_add_field(this, py_name, py_value);
}

void py_object_dump(py_object_t *this, xml_file_t *xml_file) 
{
    xml_file_dump(xml_file, "py_object");
    xml_file_push(xml_file);

    int i;
    field_t *field;
    vector_each_address (&this->field_vector, i, field) {
        char *name = field->py_name->value;
        xml_file_dump(xml_file, "%s", name);
    }

    xml_file_pop(xml_file);
}

py_string_t *py_object_to_string(py_object_t *this)
{
    if (this->py_class == py_string_class)
        return $(this);

    py_object_t *py_method = py_object_load_field(this, py_symbol__str__);
    if (py_method == NULL)
        vm_throw(py_attr_error);
    py_object_t *py_result = vm_call(py_method, 0, vm_stack + sp);
    if (py_result == NULL)
        vm_rethrow();
    if (py_result->py_class != py_string_class)
        vm_throw(py_type_error);
    return $(py_result);
}

void py_object_class_init()
{
    py_object_class = py_class_new("object");
}
