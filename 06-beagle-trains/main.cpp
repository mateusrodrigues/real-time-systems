#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"

#define WORKSIZE 4

using namespace BlackLib;

void *trem1(void *arg);
void *trem2(void *arg);
void *trem3(void *arg);
void L(int i, int j, double vel, BlackGPIO* pin1, BlackGPIO* pin2);
void *thread_potenciometro(void *arg);

pthread_mutex_t m1, m2;

BlackGPIO p1(GPIO_46,output);
BlackGPIO p2(GPIO_65,output);
BlackGPIO p3(GPIO_61,output);
BlackGPIO p4(GPIO_26,output);
BlackGPIO p5(GPIO_44,output);
BlackGPIO p6(GPIO_68,output);
BlackGPIO p7(GPIO_67,output);
BlackGPIO p8(GPIO_69,output);
BlackGPIO p9(GPIO_45,output);
BlackGPIO p10(GPIO_23,output);
BlackGPIO p11(GPIO_47,output);
BlackGPIO p12(GPIO_27,output);


ADC pot1(AIN0);
ADC pot2(AIN5);
ADC pot3(AIN6);

double vel1, vel2, vel3;

int main(int argc, char * argv[])
{
    pthread_t threads[WORKSIZE];

    pthread_mutex_init(&m1, NULL);
    pthread_mutex_init(&m2, NULL);

    pthread_create(&(threads[1]), NULL, trem1, NULL);
    pthread_create(&(threads[2]), NULL, trem2, NULL);
    pthread_create(&(threads[3]), NULL, trem3, NULL);
    pthread_create(&(threads[4]), NULL, thread_potenciometro, NULL);

    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);
    pthread_join(threads[4], NULL);

    return 0;
}

void *trem1(void *arg)
{
    while (1)
    {
        L(1, 1, vel1, &p4, &p1);
        L(1, 2, vel1, &p1, &p2);

        pthread_mutex_lock(&m1);
        L(1, 3, vel1, &p2, &p3);
        pthread_mutex_unlock(&m1);

        L(1, 4, vel1, &p3, &p4);
    }
}

void *trem2(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&m1);
        L(2, 5, vel2, &p8, &p5);
        pthread_mutex_unlock(&m1);

        L(2, 6, vel2, &p5, &p6);

        pthread_mutex_lock(&m2);
        L(2, 7, vel2, &p6, &p7);
        pthread_mutex_unlock(&m2);

        L(2, 8, vel2, &p7, &p8);
    }
}

void *trem3(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&m2);
        L(3, 9, vel3, &p12, &p9);
        pthread_mutex_unlock(&m2);

        L(3, 10, vel3, &p9, &p10);
        L(3, 11, vel3, &p10, &p11);
        L(3, 12, vel3, &p11, &p12);
    }
}

void L(int i, int j, double vel, BlackGPIO* pin1, BlackGPIO* pin2)
{
    (*pin1).setValue(low);
    (*pin2).setValue(high);
    printf("Eu sou o trem %d no trilho %d\n", i, j);
    usleep(int(100/(vel+10)  * 200000));
}

void *thread_potenciometro(void *arg){

	while (1){
		vel1 = pot1.getPercentValue();
		vel2 = pot2.getPercentValue();
		vel3 = pot3.getPercentValue();
        printf("VELOCIDADES vel1:%lf vel2:%lf vel3:%lf\n", vel1, vel2, vel3);
		usleep(1000000);
	}
	exit(0);
}

