#include "root.h"
#include "range.h"
#include "cast.h"

py_class_t *py_range_class;

py_object_t *py_range_alloc(py_class_t *py_class)
{
    py_range_t *this = py_object_alloc(sizeof(py_range_t), py_class);
    this->start = 0;
    this->end = 0;
    this->step = 1;
    this->cursor = 0;
    return $(this);
}

py_object_t *py_range_init(int argc, py_object_t *argv[])
{
    int start = 0;
    int end = 0;
    int step = 1;
   
    switch (argc) {
        case 2:
            end = cast_integer(argv[1]);
            break;

        case 3:
            start = cast_integer(argv[1]);
            end = cast_integer(argv[2]);
            break;

        case 4:
            start = cast_integer(argv[1]);
            end = cast_integer(argv[2]);
            step = cast_integer(argv[3]);
            break;
        
        default:
            vm_throw(py_type_error);
    }

    py_range_t *this = $(argv[0]);
    this->start = start;
    this->end = end;
    this->step = step;
    return $(this);
}

py_object_t *py_range_len(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_range_t *this = $(argv[0]);

    int len = (this->end - this->start) / this->step;
    py_double_t *py_result = py_double_new(len);
    return $(py_result);
}

py_object_t *py_range_iterator(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_range_t *this = $(argv[0]);
    this->cursor = this->start;
    return $(this);
}

py_object_t *py_range_next(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_range_t *this = $(argv[0]);

    if (this->cursor == this->end)
        vm_throw(py_stop_iteration);
    py_double_t *py_item = py_double_new(this->cursor);
    this->cursor += this->step;
    return $(py_item);
}

native_t py_range_natives[] = {
    {"__next__", py_range_next},
    {"__iter__", py_range_iterator},
    {"__init__", py_range_init},
    {"__len__", py_range_len},
    {NULL}
};

void py_range_class_init()
{
    py_range_class = py_class_new("range");
    py_range_class->py_alloc = py_range_alloc;
    py_class_register_natives(py_range_class, py_range_natives);
}
