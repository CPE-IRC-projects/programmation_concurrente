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
    //récupère la mémoire partagée (buffer)
    int shmidbuffer = shmget(BUFFERKEY,(BUFFERSIZE+2)*sizeof(int),0);
    int* buffer = shmat(shmidbuffer,NULL,0);
    
    while(1){
        //Génère le nombre et le temps qu'on va attendre avant de l'envoyer
        int random = (rand()%10);
        sleep(random);
        //Récupère le sémaphore inistialisé à BUFFERSIZE pour savoir si il peut écrire
        P(semwrite);
        //Prend le contrôle de la mémoire partagée
        P(mutexbuffer);
        // buffer[BUFFERSIZE] = index de la première donnée
        // buffer[BUFFERSIZE] + 1 = quantité de données présentes dans le buffer (compris entre 0 et BUFFERSIZE)
        //Ici on insère donc le chiffre au bon endroit.
        buffer[(buffer[BUFFERSIZE] + buffer[BUFFERSIZE+1])%BUFFERSIZE] = random;
        printf("envoyé %d sur l'index %d\n", random, (buffer[BUFFERSIZE] + buffer[BUFFERSIZE+1])%BUFFERSIZE);
        //Agumente la quantité de données dans le buffer
        buffer[BUFFERSIZE+1] += 1;
        //Rend le contrôle sur la mémoire
        V(mutexbuffer);
        //Ajoute dans le sémaphore pour donner la possibilité de lecture
        V(semread);

    }
    shmdt(buffer);
}