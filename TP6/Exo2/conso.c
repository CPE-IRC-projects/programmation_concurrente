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
    int semwrite = sem_get(2);
    int semread = sem_get(3);
    //récupère la mémoire partagée
    int shmidbuffer = shmget(BUFFERKEY,BUFFERSIZE*sizeof(int),0);
    int* buffer = shmat(shmidbuffer,NULL,0);

    int qte = 0;

    while(1){
        int value;
        P(semread);
        P(mutexbuffer);
        value = buffer[qte];
        qte = (qte+1)%BUFFERSIZE;
        printf("%de recu depuis buffer %d\n", qte, value);
        V(mutexbuffer);
        V(semwrite);

    }
    shmdt(buffer);
}