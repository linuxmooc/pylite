#include <stdio.h>

int loop(int n)
{
    int local;
    int i;

    local = 0;
    for (i = 0; i < n; i++)
        local += i * i;
    return local;
}

int main()
{
    printf("%d\n", loop(76543210));
    return 0;
}
