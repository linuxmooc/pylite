void loop(int range)
{
    my_init(range * 2 + 100);

    clock_t t1, t2;
    t1 = clock();

    object_t *sum = object_new(0);
    object_t *index = object_new(0);
    while (index->value < range) {
        object_t *temp;
        
        temp = object_new(index->value + 1);
        object_free(index);
        index = temp;

        temp = object_new(sum->value + 1);
        object_free(sum);
        sum = temp;
    }
    t2 = clock();

    // CLOCKS_PER_SEC represents the number of clock ticks per second
    double ticks = t2 - t1;
    double seconds = ticks / CLOCKS_PER_SEC;
    printf("sum = %d, seconds = %f\n", sum->value, seconds);
}
