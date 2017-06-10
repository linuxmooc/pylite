typedef struct {
    double value;
    int pad[56];
} object_t;

#define N 543210

object_t *object_array;
int object_count = 0;
pthread_spinlock_t spinlock;

void my_alloc_init()
{
    void *address;
    posix_memalign(&address, 64, sizeof(object_t) * N * 10);
    object_array = address;
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
}

object_t *my_alloc()
{
    pthread_spin_lock(&spinlock);
    object_t *object = object_array + object_count;
    object_count++;
    pthread_spin_unlock(&spinlock);
    return object;
}

object_t *object_new(double value)
{
    object_t *this = my_alloc();
    this->value = value;
    return this;
}

double sum = 123;
void delay()
{
    int i;
    for (i = 0; i < 123; i++)
        sum = sum * i;
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
