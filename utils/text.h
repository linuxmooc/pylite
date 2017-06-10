#ifndef _UTILS_TEXT_H
#define _UTILS_TEXT_H

typedef struct {
    char *data;
    int size;      // The number of chars, not including the trailing zero
    int capacity;
} text_t;

extern int split_string(char *string, const char *seperator, char *fields[]);
extern void text_init(text_t *this);
extern void text_destroy(text_t *this);
extern void text_put_char(text_t *this, int c);
extern int text_trim_char(text_t *this);
extern void text_printf(text_t *this, char *format, ...);
extern void text_put_string(text_t *this, char *string);

#endif
