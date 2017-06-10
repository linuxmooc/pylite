#include "root.h"
#include "tuple.h"
#include "slice.h"
#include "cast.h"
#include "gc.h"

py_class_t *py_tuple_class;

py_tuple_t *py_tuple_new()
{
    py_tuple_t *this = py_object_alloc(sizeof(py_tuple_t), py_tuple_class);
    vector_init(&this->vector);
    this->cursor = 0;
    return this;
}

void py_tuple_append(py_tuple_t *this, py_object_t *py_item)
{
    vector_push_back(&this->vector, py_item);
}

py_object_t *py_tuple_get_single(py_tuple_t *this, py_object_t *py_index)
{
    int index = cast_integer(py_index);
    py_object_t *py_item = vector_get(&this->vector, index);
    return py_item;
}

py_object_t *py_tuple_get_slice(py_tuple_t *this, py_object_t *py_index)
{
    py_slice_t *py_slice = $(py_index);
    int count = this->vector.count;
    int start;
    int stop;
    if (py_slice_parse(py_slice, &start, &stop, count) != NULL)
        vm_rethrow();

    py_tuple_t *that = py_tuple_new();
    for (int i = start; i < stop; i++) {
        py_object_t *py_item = vector_get(&this->vector, i);
        py_tuple_append(that, py_item);
    }
    return $(that);
}

py_object_t *py_tuple_get_item(int argc, py_object_t *argv[])
{
    py_tuple_t *this = $(argv[0]);
    py_object_t *py_index = argv[1];

    if (py_index->py_class == py_double_class)
        return py_tuple_get_single(this, py_index);

    if (py_index->py_class == py_slice_class)
        return py_tuple_get_slice(this, py_index);

    vm_throw(py_type_error);
}

py_object_t *py_tuple_str(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_tuple_t *this = $(argv[0]);

    text_t text;
    text_init(&text);
    text_put_char(&text, '(');

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

    text_put_char(&text, ')');
    py_string_t *py_result = py_string_new(text.data);
    text_destroy(&text);

    return $(py_result);
}

py_object_t *py_tuple_len(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_tuple_t *this = $(argv[0]);

    py_double_t *py_result = py_double_new(this->vector.count);
    return $(py_result);
}

py_object_t *py_tuple_iterator(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_tuple_t *this = $(argv[0]);
    this->cursor = 0;
    return $(this);
}

py_object_t *py_tuple_next(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_tuple_t *this = $(argv[0]);

    if (this->cursor == this->vector.count)
        vm_throw(py_stop_iteration);
    py_object_t *py_item = vector_get(&this->vector, this->cursor);
    this->cursor++;
    return py_item;
}

native_t py_tuple_natives[] = {
    {"__getitem__", py_tuple_get_item},
    {"__iter__", py_tuple_iterator},
    {"__next__", py_tuple_next},
    {"__str__", py_tuple_str},
    {"__len__", py_tuple_len},
    {NULL}
};

void py_tuple_mark(py_tuple_t *this)
{
    int i;
    py_object_t *py_item;
    vector_each (&this->vector, i, py_item)
        gc_mark(py_item);
}

void py_tuple_free(py_tuple_t *this)
{
    vector_destroy(&this->vector);
}    

void py_tuple_class_init(void)
{
    py_tuple_class = py_class_new("tuple");
    py_tuple_class->gc_mark = $(py_tuple_mark);
    py_tuple_class->gc_free = $(py_tuple_free);
    py_class_register_natives(py_tuple_class, py_tuple_natives);
}
