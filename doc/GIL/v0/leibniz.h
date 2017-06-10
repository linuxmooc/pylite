#define N 76543210
double leibniz(int start, int end)
{
    double pi = 0.0;
    double sign = 1.0;
    int i;
    for (i = start; i < end; i++) {
        pi += sign / (2 * i + 1);
        sign *= -1;
    }
    return pi * 4;
}

