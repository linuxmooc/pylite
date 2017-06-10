#include <stdio.h>
#include <stdlib.h>
#include "leibniz.h"

int main()
{
    printf("pi = %lf\n", leibniz(0, N/2) + leibniz(N/2, N));
    return 0;
}
