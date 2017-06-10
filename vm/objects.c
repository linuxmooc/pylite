#include "root.h"
#include "objects.h"

void py_classes_init(void)
{
    py_native_class_init(); // must be initialized first
    py_symbol_class_init();

    py_none_class_init();
    py_bool_class_init();

    py_double_class_init();
    py_string_class_init();

    py_lambda_class_init();
    py_method_class_init();

    py_list_class_init();
    py_dict_class_init();
    py_tuple_class_init();
    py_slice_class_init();
    py_range_class_init();

    py_file_class_init();
    py_socket_class_init();

    py_object_class_init();
}
