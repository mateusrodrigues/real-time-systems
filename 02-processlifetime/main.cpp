#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
    for (int i = 1; i <= 5; i++)
    {
        printf("pid %d, ppid %d, lifetime %d\n", getpid(), getppid(), i);
        sleep(1);

        pid_t first;
        if (i == 5)
        {
            first = fork();

            switch(first)
            {
                case -1:
                    printf("Error creating child process...\n");
                    break;
                case 0:
                    // 6 segundos
                    for (int j = 1; j <= 6; j++)
                    {
                        printf("\tpid %d, ppid %d, lifetime %d\n", getpid(), getppid(), j);
                        sleep(1);

                        pid_t second;
                        if (j == 6) 
                        {
                            second = fork();

                            switch (second)
                            {
                            case -1:
                                printf("Error creating child process...\n");
                                break;
                            case 0:
                                // 9 segundos
                                for (int k = 1; k <= 9; k++)
                                {
                                    printf("\t\tpid %d, ppid %d, lifetime %d\n", getpid(), getppid(), k);
                                    sleep(1);
                                }
                                break;
                            default:
                                // 7 segundos
                                for (int k = 1; k <= 7; k++)
                                {
                                    printf("\tpid %d, ppid %d, lifetime %d\n", getpid(), getppid(), j + k);
                                    sleep(1);
                                }
                                break;
                            }
                        }
                    }
                    break;
                default:
                    // for de mais 3 segundos
                    for (int j = 1; j <= 3; j++)
                    {
                        printf("pid %d, ppid %d, lifetime %d\n", getpid(), getppid(), i + j);
                        sleep(1);

                        pid_t second;
                        if (j == 3)
                        {
                            second = fork();

                            switch (second)
                            {
                                case -1:
                                    printf("Error creating child process...\n");
                                    break;
                                case 0:
                                    for (int k = 1; k <= 7; k++)
                                    {
                                        printf("\t\t\tpid %d, ppid %d, lifetime %d\n", getpid(), getppid(), k);
                                        sleep(1);
                                    }
                                    break;
                                default:
                                    for (int k = 1; k <= 5; k++)
                                    {
                                        printf("pid %d, ppid %d, lifetime %d\n", getpid(), getppid(), i + j + k);
                                        sleep(1);
                                    }
                                    break;
                            }
                        }
                    }
            }
        }
    }

    return 0;
}