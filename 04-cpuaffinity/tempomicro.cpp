// UFRN-CT-DCA
// Disciplina: Sistemas de Tempo Real
// Programa: tempomicro.c

#include <sys/time.h>
#include <assert.h>
#include <sched.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    pid_t filho;
    cpu_set_t my_set;
    CPU_ZERO(&my_set);

    filho = fork();
    switch(filho){
	case -1:
		break;
        case 0:
		CPU_SET(0, &my_set);
    		sched_setaffinity(getpid(), sizeof(cpu_set_t), &my_set);
		break;
	default:
		CPU_SET(1, &my_set);
    		sched_setaffinity(getpid(), sizeof(cpu_set_t), &my_set);
		break;
    }
    int i, j;
    struct timeval tempo_inicial, tempo_final;
    int tmili;

    gettimeofday(&tempo_inicial, NULL);
    
    for (j = 0; j < 10; j ++)
        for (i = 0; i < 1387634340; i ++);
    
    gettimeofday(&tempo_final, NULL);
    tmili = (int) (1000 * (tempo_final.tv_sec - tempo_inicial.tv_sec) + (tempo_final.tv_usec - tempo_inicial.tv_usec) / 1000);

    printf("tempo decorrido: %d milissegundos\n", tmili);
    return (0);
}
