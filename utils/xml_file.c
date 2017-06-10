#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "xml_file.h"

xml_file_t *xml_stdout;
xml_file_t *xml_file_new(char *path)
{
    FILE *file = fopen(path, "w");
    if (!file)
        return NULL;

    xml_file_t *this = malloc(sizeof(xml_file_t));
    this->file = file;
    this->level = 0;
    return this;
}

void xml_file_delete(xml_file_t *this)
{
    fclose(this->file);
    free(this);
}

void xml_file_indent(xml_file_t *this)
{
    for (int i = 0; i < this->level; i++)
        fprintf(this->file, "  ");
}

void xml_file_push(xml_file_t *this)
{
    this->level++;
}

void xml_file_pop(xml_file_t *this)
{
    this->level--;
}

void xml_file_printf(xml_file_t *this, char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vfprintf(this->file, format, ap);
    va_end(ap);
}

void xml_file_dump(xml_file_t *this, char *format, ...)
{
    va_list ap;
    
    xml_file_indent(this);
    va_start(ap, format);
    vfprintf(this->file, format, ap);
    fprintf(this->file, "\n");
    va_end(ap);
}
