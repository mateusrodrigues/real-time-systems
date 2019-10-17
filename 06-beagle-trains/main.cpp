#include <cstdio>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include "BlackGPIO.h"
#include "Adc.h"

#define WORKSIZE 3

using namespace BlackLib;

void *trem1(void *arg);
void *trem2(void *arg);
void *trem3(void *arg);
void L(int i, int j, int s);

pthread_mutex_t m1, m2;
BlackGPIO *leds;

int main(void)
{
    // Create LEDs array
    leds = (BlackGPIO*) malloc(12 * sizeof(BlackGPIO));

    // Initialize LEDs
    leds[0] = BlackGPIO(GPIO_46,output);
    leds[1] = BlackGPIO(GPIO_65,output);
    leds[2] = BlackGPIO(GPIO_61,output);
    leds[3] = BlackGPIO(GPIO_26,output);
    
    leds[4] = BlackGPIO(GPIO_44,output);
    leds[5] = BlackGPIO(GPIO_68,output);
    leds[6] = BlackGPIO(GPIO_67,output);
    leds[7] = BlackGPIO(GPIO_69,output);

    leds[8] = BlackGPIO(GPIO_45,output);
    leds[9] = BlackGPIO(GPIO_23,output);
    leds[10] = BlackGPIO(GPIO_51,output);
    leds[11] = BlackGPIO(GPIO_49,output);
    
    pthread_t threads[WORKSIZE];

    pthread_mutex_init(&m1, NULL);
    pthread_mutex_init(&m2, NULL);

    pthread_create(&(threads[1]), NULL, trem1, NULL);
    pthread_create(&(threads[2]), NULL, trem2, NULL);
    pthread_create(&(threads[3]), NULL, trem3, NULL);

    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);

    return 0;
}

void *trem1(void *arg)
{
    while (1)
    {
        L(1, 1, 2);
        L(1, 2, 2);

        pthread_mutex_lock(&m1);
        L(1, 3, 2);
        pthread_mutex_unlock(&m1);

        L(1, 4, 2);
    }
}

void *trem2(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&m1);
        L(2, 5, 1);
        pthread_mutex_unlock(&m1);

        L(2, 6, 1);

        pthread_mutex_lock(&m2);
        L(2, 7, 1);
        pthread_mutex_unlock(&m2);

        L(2, 8, 1);
    }
}

void *trem3(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&m2);
        L(3, 9, 3);
        pthread_mutex_unlock(&m2);

        L(3, 10, 3);
        L(3, 11, 3);
        L(3, 12, 3);
    }
}

void L(int i, int j, int s)
{
    leds[j - 1].setValue(high);
    printf("Eu sou o trem %d no trilho %d\n", i, j);
    sleep(s);
    leds[j - 1].setValue(low);
}