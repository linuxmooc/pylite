#include "root.h"
#include "string.h"
#include "tuple.h"
#include "slice.h"
#include "list.h"
#include "cast.h"

py_class_t *py_string_class;

py_string_t *py_string_new(char *value)
{
    py_string_t *this = py_object_alloc(sizeof(py_string_t), py_string_class);
    this->value = strdup(value);
    return this;
}

py_object_t *py_string_islower(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_string_t *this = $(argv[0]);

    for (char *p = this->value; *p; p++) {
        if (!islower((int)*p))
            return py_false;
    }
    return py_true;
}

py_object_t *py_string_isupper(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_string_t *this = $(argv[0]);

    for (char *p = this->value; *p; p++) {
        if (!islower((int)*p))
            return py_false;
    }
    return py_true;
}

py_object_t *py_string_lower(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_string_t *this = $(argv[0]);

    py_string_t *that = py_string_new(this->value); 
    for (char *p = that->value; *p; p++)
        *p = tolower((int)*p);
    return $(that);
}

py_object_t *py_string_upper(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_string_t *this = $(argv[0]);

    py_string_t *that = py_string_new(this->value); 
    for (char *p = that->value; *p; p++)
        *p = toupper((int)*p);
    return $(that);
}

py_object_t *py_string_add(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_string_t *this = cast_object(argv[0], string);
    py_string_t *that = cast_object(argv[1], string);

    int this_size = strlen(this->value);
    int that_size = strlen(that->value);
    char buf[this_size + that_size + 1];
    strcpy(buf, this->value);
    strcat(buf, that->value);
    py_string_t *target = py_string_new(buf);
    return $(target);
}

py_object_t *py_string_len(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_string_t *this = $(argv[0]);

    int len = strlen(this->value);
    py_double_t *py_result = py_double_new(len);
    return $(py_result);
}

py_object_t *py_string_str(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_string_t *this = $(argv[0]);
    return $(this);
}

py_string_t *arg_to_string(py_object_t *args, int index)
{
    py_object_t *arg;
    if (args->py_class == py_tuple_class) {
        py_tuple_t *py_tuple = $(args);
        if (index < 0 || index >= py_tuple->vector.count)
            vm_throw(py_index_error);
        arg = vector_get(&py_tuple->vector, index);
    } else {
        arg = args;
    }

    return py_object_to_string(arg);
}

py_object_t *py_string_eq(int argc, py_object_t *argv[])
{
    py_string_t *this = cast_object(argv[0], string);
    py_string_t *that = cast_object(argv[1], string);

    if (strcmp(this->value, that->value) == 0)
        return py_true;
    else
        return py_false;
}

py_object_t *py_string_ne(int argc, py_object_t *argv[])
{
    py_string_t *this = cast_object(argv[0], string);
    py_string_t *that = cast_object(argv[1], string);

    if (strcmp(this->value, that->value) != 0)
        return py_true;
    else
        return py_false;
}

py_object_t *py_string_mod(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_string_t *this = $(argv[0]);
    py_object_t *args = argv[1];
 
    int index = 0;
    text_t text;
    text_init(&text);
    for (char *p = this->value; *p; p++) {
        char c = *p;
        if (c != '%') {
            text_put_char(&text, c);
            continue;
        }

        p++;
        c = *p;
        if (c == '%') {
            text_put_char(&text, c);
            continue;
        }

        if (!(c == 's' || c == 'd' || c == 'f')) {
            text_destroy(&text);
            vm_throw(py_type_error);
        }

        py_string_t *py_string = arg_to_string(args, index);
        if (py_string == NULL)
            vm_rethrow();
        index++;
        text_put_string(&text, py_string->value);
    }
    py_string_t *that = py_string_new(text.data);
    text_destroy(&text);

    return $(that);
}

py_object_t *py_string_join(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_string_t *this = $(argv[0]);
    py_list_t *py_list = cast_object(argv[1], list);

    text_t text;
    text_init(&text);

    int i;
    py_object_t *py_item;
    int last = py_list->vector.count - 1;
    vector_each(&py_list->vector, i, py_item) {
        if (py_item->py_class != py_string_class) {
            text_destroy(&text);
            vm_throw(py_type_error);
        }

        text_put_string(&text, ((py_string_t *)py_item)->value);
        if (i != last)
            text_put_string(&text, this->value);
    }

    py_string_t *py_result = py_string_new(text.data);
    text_destroy(&text);
    return $(py_result);
}

py_object_t *py_string_get_single(py_string_t *this, py_object_t *py_index)
{
    int index = cast_integer(py_index);
    int len = strlen(this->value);
    if (index < 0 || index >= len)
        vm_throw(py_index_error);

    char buffer[2];
    buffer[0] = this->value[index];
    buffer[1] = 0;
    py_string_t *that = py_string_new(buffer);
    return $(that);
}

py_object_t *py_string_get_slice(py_string_t *this, py_object_t *py_index)
{
    py_slice_t *py_slice = $(py_index);
    int start;
    int stop;
    int len = strlen(this->value);
    if (py_slice_parse(py_slice, &start, &stop, len) != NULL)
        vm_rethrow();

    char buffer[stop - start + 1];
    memcpy(buffer, this->value + start, stop - start);
    buffer[stop - start] = 0;
    py_string_t *that = py_string_new(buffer);
    return $(that);
}

py_object_t *py_string_get_item(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_string_t *this = $(argv[0]);
    py_object_t *py_index = argv[1];

    if (py_index->py_class == py_double_class)
        return py_string_get_single(this, py_index);

    if (py_index->py_class == py_slice_class)
        return py_string_get_slice(this, py_index);

    vm_throw(py_type_error);
}

py_object_t *py_string_split(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_string_t *this = $(argv[0]);
    char *delimit = cast_string(argv[1]);

    int length = strlen(this->value); 
    char buffer[length];
    strcpy(buffer, this->value);

    py_list_t *py_list = py_list_new(0);
    char *token = strtok(buffer, delimit);
    while (token != NULL) {
        py_string_t *py_item = py_string_new(token);
        py_list_push_back(py_list, $(py_item));
        token = strtok(NULL, delimit);
    }

    return $(py_list);
}

py_object_t *py_string_find(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_string_t *this = $(argv[0]);
    char *needle = cast_string(argv[1]);
    char *haystack = this->value;

    int index;
    char *substr = strstr(haystack, needle);
    if (substr == NULL)
        index = -1;
    else
        index = substr - haystack;
    py_double_t *py_index = py_double_new(index);

    return $(py_index);
}

native_t py_string_natives[] = {
    {"__add__", py_string_add},
    {"__str__", py_string_str},
    {"__eq__", py_string_eq},
    {"__ne__", py_string_ne},
    {"__mod__", py_string_mod},
    {"__len__", py_string_len},
    {"__getitem__", py_string_get_item},
    {"find", py_string_find},
    {"split", py_string_split},
    {"join", py_string_join},
    {"islower", py_string_islower},
    {"isupper", py_string_isupper},
    {"lower", py_string_lower},
    {"upper", py_string_upper},
    {NULL}
};

void py_string_free(py_string_t *this)
{
    free(this->value);
}    

void py_string_class_init(void)
{
    py_string_class = py_class_new("string");
    py_string_class->gc_free = $(py_string_free);
    py_class_register_natives(py_string_class, py_string_natives);
}
