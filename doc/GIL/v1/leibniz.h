#define N 543210

typedef struct {
    double value;
} object_t;

object_t *object_new(double value)
{
    object_t *this = (object_t *) malloc(sizeof(object_t));
    this->value = value;
    return this;
}

double leibniz(int start, int end)
{
    object_t *pi = object_new(0.0);
    object_t *sign = object_new(1.0);
    int i;

    for (i = start; i < end; i++) {
        // pi += sign / (2 * i + 1);
        object_t *t0 = object_new(2 * i + 1);
        object_t *t1 = object_new(sign->value / t0->value);
        pi = object_new(pi->value + t1->value);

        // sign *= -1;
        sign = object_new(sign->value * -1);
    }
    return pi->value * 4;
}

