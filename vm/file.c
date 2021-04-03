#include "root.h"
#include "file.h"
#include "cast.h"

py_class_t *py_file_class;

py_file_t *py_file_fopen(char *path, char *mode)
{
    FILE *file = fopen(path, mode);
    if (file == NULL)
        vm_throw(py_io_error);
    py_file_t *this = py_object_alloc(sizeof(py_file_t), py_file_class);
    this->file = file;
    return this;
}

py_file_t *py_file_fdopen(int fd, char *mode)
{
    FILE *file = fdopen(fd, mode);
    if (file == NULL)
        vm_throw(py_io_error);
    py_file_t *this = py_object_alloc(sizeof(py_file_t), py_file_class);
    this->file = file;
    return this;
}

py_object_t *py_file_read(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_file_t *this = $(argv[0]);
    int count = cast_integer(argv[1]);

    char buffer[count + 1];
    count = fread(buffer, 1, count, this->file);
    buffer[count] = 0;

    py_string_t *py_string = py_string_new(buffer);
    return $(py_string);
}

py_object_t *py_file_write(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_file_t *this = $(argv[0]);
    char *string = cast_string(argv[1]);

    fputs(string, this->file); 
    return py_none;
}

py_object_t *py_file_readline(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_file_t *this = $(argv[0]);

    char buffer[1024];
    char *line = fgets(buffer, sizeof(buffer), this->file);
    if (line == NULL)
        vm_throw(py_io_error);

    py_string_t *py_string = py_string_new(line);
    return $(py_string);
}

py_object_t *py_file_flush(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_file_t *this = $(argv[0]);
    fflush(this->file);
    return py_none;
}

py_object_t *py_file_close(int argc, py_object_t *argv[])
{
    assert(argc == 1);
    py_file_t *this = $(argv[0]);
    fclose(this->file);
    return py_none;
}

py_object_t *py_file_iterator(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_file_t *this = $(argv[0]);
    return $(this);
}

py_object_t *py_file_next(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_file_t *this = $(argv[0]);

    char buffer[1024];
    char *line = fgets(buffer, sizeof(buffer), this->file);
    if (line == NULL)
        vm_throw(py_stop_iteration);
    py_string_t *py_line = py_string_new(line);
    return $(py_line);
}

native_t py_file_natives[] = {
    {"read", py_file_read},
    {"write", py_file_write},
    {"readline", py_file_readline},
    {"flush", py_file_flush},
    {"close", py_file_close},
    {"__iter__", py_file_iterator},
    {"__next__", py_file_next},
    {NULL}
};

void py_file_class_init()
{
    py_file_class = py_class_new("file");
    py_class_register_natives(py_file_class, py_file_natives);
}
