#include "../dijkstra.h"
#include "data.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    //récupère les semaphores / mutexs
    int mutexbuffer = sem_get(1);
    int mutexqte = sem_get(4);
    int mutexindex = sem_get(5);
    //semaphore pour savoir si l'écriture est disponible
    int semwrite = sem_get(2);
    //semaphore pour savoir si la lecture est disponible
    int semread = sem_get(3);
    //récupère la mémoire partagée (buffer)
    int shmidbuffer = shmget(BUFFERKEY,BUFFERSIZE*sizeof(int),0);
    int* buffer = shmat(shmidbuffer,NULL,0);
    //récupère la seconde mémoire partagée (la quantité de données dans le tableau)
    int shmidqte = shmget(QTEKEY,sizeof(int),0);
    int* qte = shmat(shmidqte,NULL,0);
    //récupère la troisème mémoire partagée (l'index de la première donnée dans le tableau)
    int shmidindex = shmget(INDEXKEY,sizeof(int),0);
    int* index = shmat(shmidindex,NULL,0);
    while(1){
        int random = (rand()%10);
        sleep(random);
        P(semwrite);
        P(mutexbuffer);
        P(mutexqte);
        P(mutexindex);
        buffer[(*index + *qte)%BUFFERSIZE] = random;
        printf("envoyé %d sur l'index %d\n", random, (*index + *qte)%BUFFERSIZE);
        V(mutexindex);
        *qte = *qte+1;
        // printf("%de push dans buffer %d\n", *qte, random);
        V(mutexqte);
        V(mutexbuffer);
        V(semread);

    }
    shmdt(buffer);
    shmdt(qte);
    shmdt(index);
}