#ifndef _UTILS_XML_FILE_H
#define _UTILS_XML_FILE_H

#include <stdio.h>
typedef struct {
    FILE *file;
    int level;
} xml_file_t;

extern xml_file_t *xml_stdout;
extern xml_file_t *xml_file_new(char *path);
extern void xml_file_delete(xml_file_t *this);
extern void xml_file_indent(xml_file_t *this);
extern void xml_file_push(xml_file_t *this);
extern void xml_file_pop(xml_file_t *this);
extern void xml_file_printf(xml_file_t *this, char *format, ...);
extern void xml_file_dump(xml_file_t *this, char *format, ...);

#endif
