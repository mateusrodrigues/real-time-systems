#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define WORKSIZE 2

void *trem1(void *arg);
void *trem2(void *arg);
void L(int i, int j, int s);

pthread_mutex_t m1, m2;

int main(void)
{
    pthread_t threads[WORKSIZE];

    pthread_mutex_init(&m1, NULL);
    pthread_mutex_init(&m2, NULL);

    pthread_create(&(threads[1]), NULL, trem1, NULL);
    pthread_create(&(threads[2]), NULL, trem2, NULL);

    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    return 0;
}

void *trem1(void *arg)
{
    while (1)
    {
        L(1, 1, 1);
        L(1, 2, 1);

        pthread_mutex_lock(&m1);
        L(1, 3, 10);
        pthread_mutex_unlock(&m2);

        L(1, 4, 1);
    }
}

void *trem2(void *arg)
{
    while (1)
    {
        L(2, 5, 1);
        L(2, 6, 1);

        pthread_mutex_lock(&m2);
        L(2, 7, 1);
        pthread_mutex_unlock(&m1);

        L(2, 8, 1);
    }
}

void L(int i, int j, int s)
{
    printf("Eu sou o trem %d no trilho %d\n", i, j);
    sleep(s);
}