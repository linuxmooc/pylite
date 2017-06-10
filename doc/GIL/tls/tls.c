#include <stdio.h>

__thread int tls;
__thread int i;

int loop(int n)
{
    tls = 0;
    for (i = 0; i < n; i++)
        tls += i * i;
    return tls;
}

int main()
{
    printf("%d\n", loop(76543210));
    return 0;
}
