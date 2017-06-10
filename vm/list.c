#include "root.h"
#include "list.h"
#include "slice.h"
#include "cast.h"
#include "gc.h"

py_class_t *py_list_class;

py_list_t *py_list_new(int count)
{
    py_list_t *this = py_object_alloc(sizeof(py_list_t), py_list_class);
    vector_init(&this->vector);
    this->cursor = 0;
    for (int i = 0; i < count; i++)
        vector_push_back(&this->vector, py_none);
    return this;
}

void py_list_push_back(py_list_t *this, py_object_t *py_item)
{
    vector_push_back(&this->vector, py_item);
}

py_object_t *py_list_alloc(py_class_t *py_class)
{
    py_list_t *this = py_object_alloc(sizeof(py_list_t), py_class);
    vector_init(&this->vector);
    this->cursor = 0;
    return $(this);
}

py_object_t *py_list_init(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_list_t *this = $(argv[0]);
    return py_none;
}

py_object_t *py_list_append(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_list_t *this = $(argv[0]);
    py_object_t *py_item = argv[1];
    py_list_push_back(this, py_item);
    return py_none;
}

// insert item before index
py_object_t *py_list_insert(int argc, py_object_t *argv[])
{
    assert_argc(argc, 3);
    py_list_t *this = $(argv[0]);
    int index = cast_range(argv[1], 0, this->vector.count);
    py_object_t *py_item = argv[2];

    vector_push_back(&this->vector, py_none);
    py_object_t **vector = this->vector.data;
    for (int i = this->vector.count - 1; i > index; i--)
        vector[i] = vector[i - 1];
    vector[index] = py_item;
    return py_none;
}

py_object_t *py_list_get_single(py_list_t *this, py_object_t *py_index)
{
    int index = cast_range(py_index, 0, this->vector.count);
    py_object_t *py_item = vector_get(&this->vector, index);
    return py_item;
}

py_object_t *py_list_get_slice(py_list_t *this, py_object_t *py_index)
{
    py_slice_t *py_slice = $(py_index);
    int count = this->vector.count;
    int start;
    int stop;
    if (py_slice_parse(py_slice, &start, &stop, count) != NULL)
        vm_rethrow();

    py_list_t *that = py_list_new(stop - start);
    for (int i = start; i < stop; i++) {
        int j = i - start; 
        that->vector.data[j] = this->vector.data[i];
    }
    return $(that);
}

py_object_t *py_list_get_item(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_list_t *this = $(argv[0]);
    py_object_t *py_index = argv[1];

    if (py_index->py_class == py_double_class)
        return py_list_get_single(this, py_index);

    if (py_index->py_class == py_slice_class)
        return py_list_get_slice(this, py_index);

    vm_throw(py_type_error);
}

py_object_t *py_list_set_item(int argc, py_object_t *argv[])
{
    assert_argc(argc, 3);
    py_list_t *this = $(argv[0]);
    int index = cast_range(argv[1], 0, this->vector.count);
    py_object_t *py_item = argv[2];

    vector_set(&this->vector, index, py_item);
    return py_none;
}

py_object_t *py_list_str(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_list_t *this = $(argv[0]);

    text_t text;
    text_init(&text);
    text_put_char(&text, '[');

    int i;
    py_object_t *py_item;
    vector_each (&this->vector, i, py_item) {
        py_string_t *py_string = py_object_to_string(py_item);
        if (py_string == NULL) {
            text_destroy(&text);
            vm_rethrow();
        }

        text_put_string(&text, py_string->value);
        if (i != this->vector.count - 1)
            text_put_string(&text, ", ");
    }

    text_put_char(&text, ']');
    py_string_t *py_result = py_string_new(text.data);
    text_destroy(&text);

    return $(py_result);
}

py_object_t *py_list_len(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_list_t *this = $(argv[0]);

    py_double_t *py_result = py_double_new(this->vector.count);
    return $(py_result);
}

py_object_t *py_list_iterator(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_list_t *this = $(argv[0]);
    this->cursor = 0;
    return $(this);
}

py_object_t *py_list_next(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_list_t *this = $(argv[0]);

    if (this->cursor == this->vector.count)
        vm_throw(py_stop_iteration);
    py_object_t *py_item = vector_get(&this->vector, this->cursor);
    this->cursor++;
    return py_item;
}

native_t py_list_natives[] = {
    {"append", py_list_append},
    {"insert", py_list_insert},
    {"__init__", py_list_init},
    {"__iter__", py_list_iterator},
    {"__next__", py_list_next},
    {"__getitem__", py_list_get_item},
    {"__setitem__", py_list_set_item},
    {"__str__", py_list_str},
    {"__len__", py_list_len},
    {NULL}
};

void py_list_mark(py_list_t *this)
{
    int i;
    py_object_t *py_item;
    vector_each (&this->vector, i, py_item)
        gc_mark(py_item);
}

void py_list_free(py_list_t *this)
{
    vector_destroy(&this->vector);
}    

void py_list_class_init(void)
{
    py_list_class = py_class_new("list");
    py_list_class->py_alloc = py_list_alloc;
    py_list_class->gc_mark = $(py_list_mark);
    py_list_class->gc_free = $(py_list_free);
    py_class_register_natives(py_list_class, py_list_natives);
}
