#include "../dijkstra.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char **argv)
{
    if(argc < 2){
        exit(-1);
    }
    //transforme en int 
    int nbProcessus = atoi(argv[1]);
    //récupère les semaphores
    int semid = sem_get(1);
    int mutex = sem_get(2);
    //récupère la mémoire partagée
    int shmid = shmget(5942,sizeof(int),0);
    int* mem = shmat(shmid,NULL,0);
    //attend pcq c'est drole
    int random = (rand()%20)+1;
    sleep(random);
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