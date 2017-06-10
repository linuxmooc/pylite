#ifndef _VM_OBJECTS_H
#define _VM_OBJECTS_H

#include "object.h"
#include "class.h"

#include "double.h"
#include "bool.h"
#include "symbol.h"
#include "string.h"
#include "lambda.h"
#include "method.h"
#include "none.h"
#include "list.h"
#include "dict.h"
#include "tuple.h"
#include "slice.h"
#include "range.h"
#include "native.h"
#include "file.h"
#include "socket.h"

extern void py_classes_init(void);
#endif
