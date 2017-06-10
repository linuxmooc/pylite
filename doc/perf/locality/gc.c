#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "object.h"
#define object_free(object)
#include "loop.h"

int main()
{
    loop(16 * 1024 * 1024);
    return 0;
}
