#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"
#include <sys/socket.h>
#include <arpa/inet.h>

#define WORKSIZE        6
#define MULTICAST_ADDR  "225.0.0.37"

using namespace BlackLib;

void *trem1(void *arg);
void *trem2(void *arg);
void *trem3(void *arg);
void *trem4(void *arg);
void L(int i, int j, double vel, BlackGPIO* pin1, BlackGPIO* pin2);
void *thread_envia(void *arg);
void *thread_recebe(void *arg);

pthread_mutex_t m1, m2, m3, m4, m5;

BlackGPIO p1(GPIO_46,output);
BlackGPIO p2(GPIO_61,output);
BlackGPIO p3(GPIO_65,output);

BlackGPIO p4(GPIO_69,output);
BlackGPIO p5(GPIO_67,output);
BlackGPIO p6(GPIO_68,output);
BlackGPIO p7(GPIO_44,output);

BlackGPIO p8(GPIO_47,output);
BlackGPIO p9(GPIO_23,output);
BlackGPIO p10(GPIO_45,output);

BlackGPIO p11(GPIO_7,output);
BlackGPIO p12(GPIO_22,output);
BlackGPIO p13(GPIO_26,output);
BlackGPIO p14(GPIO_27,output);

ADC pot1(AIN3);
ADC pot2(AIN2);
ADC pot3(AIN1);
ADC pot4(AIN4);

double vel1, vel2, vel3, vel4;
unsigned short portaLeitura = 6601;

int main(int argc, char * argv[])
{
    pthread_t threads[WORKSIZE];

    pthread_mutex_init(&m1, NULL);
    pthread_mutex_init(&m2, NULL);
    pthread_mutex_init(&m3, NULL);
    pthread_mutex_init(&m4, NULL);
    pthread_mutex_init(&m5, NULL);

    pthread_create(&(threads[0]), NULL, trem1, NULL);
    pthread_create(&(threads[1]), NULL, trem2, NULL);
    pthread_create(&(threads[2]), NULL, trem3, NULL);
    pthread_create(&(threads[3]), NULL, trem4, NULL);
    pthread_create(&(threads[4]), NULL, thread_envia, NULL);
    pthread_create(&(threads[5]), NULL, thread_recebe, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);
    pthread_join(threads[4], NULL);
    pthread_join(threads[5], NULL);

    return 0;
}

void *trem1(void *arg)
{
    while (1)
    {
        L(1, 1, vel1, &p3, &p1);

        pthread_mutex_lock(&m3);
        pthread_mutex_lock(&m1);
        L(1, 2, vel1, &p1, &p2);
        pthread_mutex_unlock(&m1);
        L(1, 3, vel1, &p2, &p3);
        pthread_mutex_unlock(&m3);
    }
}

void *trem2(void *arg)
{
    while (1)
    {
        L(2, 4, vel2, &p7, &p4);
        
        pthread_mutex_lock(&m2);
        L(2, 5, vel2, &p4, &p5);
        pthread_mutex_unlock(&m2);

        pthread_mutex_lock(&m4);
        L(2, 6, vel2, &p5, &p6);
        pthread_mutex_unlock(&m4);

        pthread_mutex_lock(&m1);
        L(2, 7, vel2, &p6, &p7);
        pthread_mutex_unlock(&m1);
    }
}

void *trem3(void *arg)
{
    while (1)
    {
        L(3, 8, vel3, &p10, &p8);

        pthread_mutex_lock(&m2);
        pthread_mutex_lock(&m5);
        L(3, 9, vel3, &p8, &p9);
        pthread_mutex_unlock(&m5);
        L(3, 10, vel3, &p9, &p10);
        pthread_mutex_unlock(&m2);
    }
}

void *trem4(void *arg)
{
    while (1)
    {
        L(4, 11, vel4, &p14, &p11);

        pthread_mutex_lock(&m3);
        L(4, 12, vel4, &p11, &p12);
        pthread_mutex_unlock(&m3);

        pthread_mutex_lock(&m4);
        L(4, 13, vel4, &p12, &p13);
        pthread_mutex_unlock(&m4);

        pthread_mutex_lock(&m5);
        L(4, 14, vel4, &p13, &p14);
        pthread_mutex_unlock(&m5);
    }
}

void L(int i, int j, double vel, BlackGPIO* pin1, BlackGPIO* pin2)
{
    (*pin1).setValue(low);
    (*pin2).setValue(high);
    //printf("Eu sou o trem %d no trilho %d\n", i, j);
    usleep(int(100 / ((vel * 100) + 10) * 200000));
}

void *thread_envia(void *arg){
	int sockfd;
	int len;
	socklen_t len_recv;
	struct sockaddr_in address;
	int result;
    double velocidades[4];

	unsigned short porta = 6601;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	address.sin_port = htons(porta);
	len = sizeof(address);

	while (1){
		velocidades[0] = pot1.getPercentValue() / 100;
		velocidades[1] = pot2.getPercentValue() / 100;
		velocidades[2] = pot3.getPercentValue() / 100;
		velocidades[3] = pot4.getPercentValue() / 100;
		
        printf("ENVIO vel1:%lf vel2:%lf vel3:%lf vel4:%lf\n", velocidades[0],
            velocidades[1], velocidades[2], velocidades[3]);

		sendto(sockfd, &velocidades, sizeof(velocidades), 0, (struct sockaddr *)&address, len);
		
		usleep(1000000);
	}

	exit(0);
}

void *thread_recebe(void *arg){
	int server_sockfd, client_sockfd;
	size_t server_len;
	socklen_t client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	struct ip_mreq mreq;

	if ((server_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("socket error.\n");
		exit(1);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(portaLeitura);
	
	server_len = sizeof(server_address);

	if (bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0){
		perror("bind error.\n");
		exit(1);
	}

	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(server_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
		perror("setsockopt");
		exit(1);
	}
	printf(" IPPROTO_IP = %d\n", IPPROTO_IP);
	printf(" SOL_SOCKET = %d\n", SOL_SOCKET);
	printf(" IP_ADD_MEMBERSHIP = %d \n", IP_ADD_MEMBERSHIP);
	
	double velocidades[4];
	while (1)
	{
		client_len = sizeof(client_address);
		if (recvfrom(server_sockfd, &velocidades, sizeof(velocidades), 0, (struct sockaddr *)&client_address, &client_len) < 0)
		{
			perror("RECVFROM() error.\n");
			exit(1);
		}

		vel1 = velocidades[0];
		vel2 = velocidades[1];
		vel3 = velocidades[2];
		vel4 = velocidades[3];

		printf ("RECEBIMENTO: %f %f %f %f\n", velocidades[0], velocidades[1], velocidades[2], velocidades[3]);
	}

	close(server_sockfd);
	exit(0);
}