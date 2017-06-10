#ifndef _COM_ROOT_H
#define _COM_ROOT_H

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "utils/list.h"
#include "utils/stack.h"
#include "utils/vector.h"
#include "utils/text.h"
#include "utils/xml_file.h"

#define pointer_add(pointer, delta) (((unsigned long)pointer) + delta)
#define pointer_sub(pointer, delta) (((unsigned long)pointer) - delta)

#define trace(...)                                                           \
    do {                                                                     \
        fprintf(stderr, "%s:%d:", __FILE__, __LINE__);                       \
        fprintf(stderr, __VA_ARGS__);                                        \
    } while (0)

#define fatal(message)                                                       \
    do {                                                                     \
        perror(message);                                                     \
        exit(EXIT_FAILURE);                                                  \
    } while (0)

#define array_count(array) sizeof(array)/sizeof(array[0])

#endif
