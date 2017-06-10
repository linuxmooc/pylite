#include "root.h"
#include "dict.h"
#include "list.h"
#include "cast.h"
#include "gc.h"

py_class_t *py_dict_class;

py_dict_t *py_dict_new()
{
    py_dict_t *this = py_object_alloc(sizeof(py_dict_t), py_dict_class);
    vector_init(&this->vector);
    return this;
}

bool py_object_equals(py_object_t *this, py_object_t *that)
{
    if (this->py_class == py_double_class) {
        if (that->py_class != py_double_class)
            return false;
        double this_value = py_object_to_double(this);
        double that_value = py_object_to_double(that);
        return this_value == that_value;
    }

    if (this->py_class == py_string_class) {
        if (that->py_class != py_string_class)
            return false;
        char *this_value = ((py_string_t *)this)->value;
        char *that_value = ((py_string_t *)that)->value;
        return strcmp(this_value, that_value) == 0;
    }

    assert(false);
    return false;
}

py_object_t *py_dict_alloc(py_class_t *py_class)
{
    py_dict_t *this = py_object_alloc(sizeof(py_dict_t), py_class);
    vector_init(&this->vector);
    return $(this);
}

py_object_t *py_dict_init(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_dict_t *this = $(argv[0]);
    return py_none;
}

py_object_t *py_dict_get_item(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_dict_t *this = $(argv[0]);
    py_object_t *py_index = argv[1];

    for (int i = 0; i < this->vector.count; i++) {
        py_pair_t *py_pair = this->vector.data + i;
        if (py_object_equals(py_pair->key, py_index))
            return py_pair->value;
    }
    vm_throw(py_key_error);
}

py_object_t *py_dict_set_item(int argc, py_object_t *argv[])
{
    assert_argc(argc, 3);
    py_dict_t *this = $(argv[0]);
    py_object_t *py_index = argv[1];
    py_object_t *py_item = argv[2];

    for (int i = 0; i < this->vector.count; i++) {
        py_pair_t *py_pair = this->vector.data + i;
        if (py_object_equals(py_pair->key, py_index)) {
            py_pair->value = py_item;
            return py_none;
        }
    }

    py_pair_t py_pair;
    py_pair.key = py_index;
    py_pair.value = py_item;
    vector_push_back(&this->vector, py_pair);
    return py_none;
}

py_object_t *py_dict_str(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_dict_t *this = $(argv[0]);

    text_t text;
    text_init(&text);
    text_put_char(&text, '{');

    int i;
    py_pair_t *py_pair;
    vector_each_address (&this->vector, i, py_pair) {
        py_pair_t *py_pair = this->vector.data + i;

        // print key
        py_string_t *py_key = py_object_to_string(py_pair->key);
        if (py_key == NULL) {
            text_destroy(&text);
            vm_rethrow();
        }
        text_put_string(&text, py_key->value);

        // print :
        text_put_string(&text, ": ");

        // print value
        py_string_t *py_value = py_object_to_string(py_pair->value);
        if (py_value == NULL) {
            text_destroy(&text);
            vm_rethrow();
        }
        text_put_string(&text, py_value->value);

        if (i != this->vector.count - 1)
            text_put_string(&text, ", ");
    }

    text_put_char(&text, '}');
    py_string_t *py_result = py_string_new(text.data);
    text_destroy(&text);

    return $(py_result);
}

py_object_t *py_dict_len(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_dict_t *this = $(argv[0]);

    py_double_t *py_result = py_double_new(this->vector.count);
    return $(py_result);
}

py_object_t *py_dict_items(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_dict_t *this = $(argv[0]);
    py_list_t *py_list = py_list_new(0);

    int i;
    py_pair_t *py_pair;
    vector_each_address (&this->vector, i, py_pair) {
        py_list_t *py_item = py_list_new(0);
        py_list_push_back(py_item, py_pair->key);
        py_list_push_back(py_item, py_pair->value);
        py_list_push_back(py_list, $(py_item));
    }
    return $(py_list);
}

py_object_t *py_dict_keys(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_dict_t *this = $(argv[0]);
    py_list_t *py_list = py_list_new(0);

    int i;
    py_pair_t *py_pair;
    vector_each_address (&this->vector, i, py_pair) {
        py_object_t *key = py_pair->key; 
        py_list_push_back(py_list, key);
    }
    return $(py_list);
}

py_object_t *py_dict_values(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_dict_t *this = $(argv[0]);
    py_list_t *py_list = py_list_new(0);

    int i;
    py_pair_t *py_pair;
    vector_each_address (&this->vector, i, py_pair) {
        py_object_t *value = py_pair->value; 
        py_list_push_back(py_list, value);
    }
    return $(py_list);
}

native_t py_dict_natives[] = {
    {"__init__", py_dict_init},
    {"__getitem__", py_dict_get_item},
    {"__setitem__", py_dict_set_item},
    {"__str__", py_dict_str},
    {"__len__", py_dict_len},
    {"items", py_dict_items},
    {"keys", py_dict_keys},
    {"values", py_dict_values},
    {NULL}
};

void py_dict_mark(py_dict_t *this)
{
    int i;
    py_pair_t *py_pair;
    vector_each_address (&this->vector, i, py_pair) {
        gc_mark(py_pair->key);
        gc_mark(py_pair->value);
    }
}

void py_dict_free(py_dict_t *this)
{
    vector_destroy(&this->vector);
}    

void py_dict_class_init(void)
{
    py_dict_class = py_class_new("dict");
    py_dict_class->py_alloc = py_dict_alloc;
    py_dict_class->gc_mark = $(py_dict_mark);
    py_dict_class->gc_free = $(py_dict_free);
    py_class_register_natives(py_dict_class, py_dict_natives);
}
