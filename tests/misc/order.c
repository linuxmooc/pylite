#include <stdio.h>

int yindex()
{
    puts("index");
    return 0;
}

int value()
{
    puts("value");
    return 0;
}

int *array()
{
    static int a[] = {0};
    puts("array");
    return a;
}

int left()
{
    puts("left");
    return 0;
}

int right()
{
    puts("right");
    return 0;
}

void add(int a, int b)
{
}

int main()
{
    array()[yindex()] = value();
    add(left(), right());
    return 0;
}
