#include "../dijkstra.h"
#include "data.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    //récupère les semaphores
    int mutexbuffer = sem_get(1);
    int mutexqte = sem_get(4);
    int mutexindex = sem_get(5);

    int semwrite = sem_get(2);
    int semread = sem_get(3);
    //récupère la mémoire partagée
    int shmidbuffer = shmget(BUFFERKEY,BUFFERSIZE*sizeof(int),0);
    int* buffer = shmat(shmidbuffer,NULL,0);
    //récupère la seconde mémoire partagée (la quantité de données dans le tableau)
    int shmidqte = shmget(QTEKEY,sizeof(int),0);
    int* qte = shmat(shmidqte,NULL,0);
    //récupère la troisème mémoire partagée (l'index de la première donnée dans le tableau)
    int shmidindex = shmget(INDEXKEY,sizeof(int),0);
    int* index = shmat(shmidindex,NULL,0);

    while(1){
        int value;
        P(semread);
        P(mutexbuffer);
        P(mutexindex);
        value = buffer[*index];
        printf("reçu %d depuis l'index %d\n",value, *index);
        *index = (*index+1)%BUFFERSIZE;
        V(mutexindex);
        P(mutexqte);
        *qte -= 1;
        V(mutexqte);
        V(mutexbuffer);
        V(semwrite);

    }
    shmdt(buffer);
    shmdt(qte);
    shmdt(index);
}