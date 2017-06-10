#ifndef _PY_FILE_H
#define _PY_FILE_H

#include "object.h"
#include "class.h"

typedef struct py_file {
    py_object_member_list;
    FILE *file;
} py_file_t;

extern py_class_t *py_file_class;
extern void py_file_class_init();
extern py_file_t *py_file_fopen(char *path, char *mode);
extern py_file_t *py_file_fdopen(int fd, char *mode);

#endif
