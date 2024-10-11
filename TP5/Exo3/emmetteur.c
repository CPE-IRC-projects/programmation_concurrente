#include "../dijkstra.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int semid = sem_get(1);
    int semid2 = sem_get(2);
    int attenteEmmeteur = sem_get(3);
    pid_t pid = getpid();
    printf("Emmetteur %d démarre\n",pid);
    P(attenteEmmeteur);
    printf("Emmetteur %d en attente premier recepteur\n",pid);
    P(semid);
    printf("Emmetteur %d en attente second recepteur\n",pid);
    P(semid);
    V(semid2);
    V(semid2);
    printf("Emmetteur %d libéré\n",pid);
    V(attenteEmmeteur);
}
