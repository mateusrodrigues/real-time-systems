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
void L(int i, int j, int s, BlackGPIO* pin1, BlackGPIO* pin2);

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
    pthread_t potenciometro;

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
        L(1, 1, 2, &p4, &p1);
        L(1, 2, 2, &p1, &p2);

        pthread_mutex_lock(&m1);
        L(1, 3, 2, &p2, &p3);
        pthread_mutex_unlock(&m1);

        L(1, 4, 2, &p3, &p4);
    }
}

void *trem2(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&m1);
        L(2, 5, 1, &p8, &p5);
        pthread_mutex_unlock(&m1);

        L(2, 6, 1, &p5, &p6);

        pthread_mutex_lock(&m2);
        L(2, 7, 1, &p6, &p7);
        pthread_mutex_unlock(&m2);

        L(2, 8, 1, &p7, &p8);
    }
}

void *trem3(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&m2);
        L(3, 9, 3, &p12, &p9);
        pthread_mutex_unlock(&m2);

        L(3, 10, 3, &p9, &p10);
        L(3, 11, 3, &p10, &p11);
        L(3, 12, 3, &p11, &p12);
    }
}

void L(int i, int j, int s, BlackGPIO* pin1, BlackGPIO* pin2)
{
    (*pin1).setValue(low);
    (*pin2).setValue(high);
    printf("Eu sou o trem %d no trilho %d\n", i, j);
    sleep(s);
}

void *thread_potenciometro(void *arg){

	while (1){
		vel1 = pot1.getPercentValue();
		vel2 = pot2.getPercentValue();
		vel3 = pot3.getPercentValue();
		//cout <<"ESCRITA: "<< vel1 <<"  "<< vel2 <<"  "<< vel3 << endl;
        printf("ESCRITA vel1:%lf vel2:%lf vel3:%lf\n", vel1, vel2, vel3);
		//sleep(1);
		usleep(1000000);
	}
	exit(0);
}

