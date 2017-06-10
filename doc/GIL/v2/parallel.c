#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "leibniz.h"

struct param {
    int start;
    int end;
    double result;
};

void *slave(void *arg)
{
    struct param *param = (struct param *)arg;
    param->result = leibniz(param->start, param->end);
    return param;
}

int main()
{
    pthread_t tid0;
    pthread_t tid1;
    struct param *param0;
    struct param *param1;

    my_alloc_init();

    param0 = malloc(sizeof(struct param));
    param0->start = 0;
    param0->end = N/2;
    pthread_create(&tid0, NULL, slave, param0);

    param1 = malloc(sizeof(struct param));
    param1->start = N/2;
    param1->end = N;
    pthread_create(&tid1, NULL, slave, param1);

    pthread_join(tid0, NULL);
    pthread_join(tid1, NULL);

    printf("pi = %lf\n", param0->result + param1->result);
    return 0;
}
