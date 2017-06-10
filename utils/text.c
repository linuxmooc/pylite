#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "text.h"

// The contents of input string will be destroyed
int split_string(char *string, const char *seperator, char *fields[])
{
    int fields_count = 0;
    char *word, *word_save;
    word = strtok_r(string, seperator, &word_save);        
    while (word) {
        fields[fields_count++] = word;
        word = strtok_r(NULL, seperator, &word_save);
    }
    return fields_count;
}

#define TEXT_CAPACITY 64
void text_init(text_t *this)
{
    this->data = malloc(TEXT_CAPACITY);
    this->size = 0;
    this->capacity = TEXT_CAPACITY;
    this->data[this->size] = 0;
}

void text_destroy(text_t *this)
{
    free(this->data);
}

void text_grow(text_t *this)
{
    int old_capacity = this->capacity;
    int new_capacity = old_capacity * 2;

    this->data = realloc(this->data, new_capacity);
    this->size = this->size;
    this->capacity = new_capacity;
}

void text_put_char(text_t *this, int c)
{
    assert(c != EOF);
    if (this->size + 1 == this->capacity)
        text_grow(this);
    this->data[this->size] = c;
    this->size++;
    this->data[this->size] = 0;
}

int text_trim_char(text_t *this)
{
    if (this->size == 0)
        return 0;
    this->size--;
    char c = this->data[this->size];
    this->data[this->size] = 0;
    return c;
}

void text_put_buffer(text_t *this, char *buffer, int length)
{
    while (this->size + length + 1 > this->capacity)
        text_grow(this);

    memcpy(this->data + this->size, buffer, length);
    this->size += length;
    this->data[this->size] = 0;

    assert(this->size + 1 <= this->capacity);
}

void text_printf(text_t *this, char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char buffer[256];
    int length = vsprintf(buffer, format, ap);
    assert(length < sizeof(buffer));
    text_put_buffer(this, buffer, length);

    va_end(ap);
}

void text_put_string(text_t *this, char *string)
{
    int length = strlen(string);
    text_put_buffer(this, string, length);
}
