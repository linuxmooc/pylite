#include <stdio.h>

int global;
int i;

int loop(int n)
{
    global = 0;
    for (i = 0; i < n; i++)
        global += i * i;
    return global;
}

int main()
{
    printf("%d\n", loop(76543210));
    return 0;
}
