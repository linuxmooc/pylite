#include "root.h"
#include "slice.h"
#include "cast.h"

py_class_t *py_slice_class;

py_slice_t *py_slice_new(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_object_t *start = argv[0];
    py_object_t *stop = argv[1];

    py_slice_t *this = py_object_alloc(sizeof(py_slice_t), py_slice_class);
    this->start = start;
    this->stop = stop;
    py_object_set_field($(this), py_symbol_start, start);
    py_object_set_field($(this), py_symbol_stop, stop);
    return this;
}

void py_slice_class_init()
{
    py_slice_class = py_class_new("slice");
}

py_object_t *py_slice_parse(py_slice_t *this, int *_start, int *_stop, int size)
{
    int start = cast_integer(this->start);
    if (start < 0)
        vm_throw(py_index_error);

    int stop;
    if (this->stop == py_none) {
        stop = size;
    } else {
        stop = cast_integer(this->stop);
        if (stop < 0)
            stop += size;
    }

    if (start > stop)
        vm_throw(py_index_error);

    *_start = start;
    *_stop = stop;
    return NULL;
}
