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
    //semaphore pour savoir si l'écriture est disponible
    int semwrite = sem_get(2);
    //semaphore pour savoir si la lecture est disponible
    int semread = sem_get(3);
    //récupère la mémoire partagée
    int shmidbuffer = shmget(BUFFERKEY,(BUFFERSIZE+2)*sizeof(int),0);
    int* buffer = shmat(shmidbuffer,NULL,0);

    while(1){
        //Attend d'avoir la possibilité de lire
        P(semread);
        //Prend le contrôle de la mémoire
        P(mutexbuffer);
        //Récupère la première donnée (l'index de la première donnée est stockée dans buffer[BUFFERSIZE])
        int value = buffer[buffer[BUFFERSIZE]];
        printf("reçu %d depuis l'index %d\n",value, buffer[BUFFERSIZE]);
        //Incrémente l'index (le modulo pour remettre à 6 pour ne pas arriver plus loin que BUFFERSIZE-1)
        buffer[BUFFERSIZE] = (buffer[BUFFERSIZE]+1)%BUFFERSIZE;
        //Décrémente la quantité de données dans le buffer
        buffer[BUFFERSIZE+1] -= 1;
        //Libère la mémoire
        V(mutexbuffer);
        //Informe qu'une place en écriture est disponible
        V(semwrite);

    }
    shmdt(buffer);
}