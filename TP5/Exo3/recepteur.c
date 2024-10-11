#include "../dijkstra.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    int semid = sem_get(1);
    int semid2 = sem_get(2);
    pid_t pid = getpid();
    printf("Recepteur %d démarre\n",pid);
    V(semid);
    printf("Recepteur %d en attente\n",pid);
    P(semid2);
    printf("Recepteur %d libéré\n",pid);
}
