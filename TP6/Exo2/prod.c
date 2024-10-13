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
    int mutexbufferqte = sem_get(2);
    int mutexreadwrite = sem_get(3);
    //récupère la mémoire partagée
    int shmidbuffer = shmget(BUFFERKEY,BUFFERSIZE*sizeof(int),0);
    int* buffer = shmat(shmidbuffer,NULL,0);
    int shmidbufferqte = shmget(BUFFERQTEKEY, sizeof(int),0);
    int* qte = shmat(shmidbufferqte, NULL, 0);
    int shmireadwrite = shmget(READWRITEKEY, sizeof(int),0);
    int* readwrite = shmat(shmidbufferqte, NULL, 0);
    while(1){
        int random = (rand()%10);
        sleep(random);
        P(shmireadwrite);
        if(*readwrite<BUFFERSIZE){
            P(mutexbuffer);
            P(mutexbufferqte);
            buffer[*qte] = random;
            *qte = (*qte+1)%BUFFERSIZE;
            V(mutexbufferqte);
            V(mutexbuffer);
            *readwrite += 1;
        }
        V(shmireadwrite);

    }
    //récupère le droit sur le mutex pour mettre à jour la donnée
    P(mutex);
    *mem += 1;
    //vérifie si il est le dernier à arriver
    if(*mem == nbProcessus)
    {
        for(int i = 0; i<nbProcessus; i++){
            V(semid);
        }
    }
    printf("%d arrivé\n",mem[0]);
    //rend le mutex
    V(mutex);
    //attend que tout le monde soit arrivé
    P(semid);
    printf("reparti\n");
    shmdt(mem);
}