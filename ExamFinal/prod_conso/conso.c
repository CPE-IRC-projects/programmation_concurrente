#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "dijkstra.h"
#include "commun.h"

int main () {
     int sem_prod = sem_create (CLE_SEM_PROD ,SEM);
     int sem_conso = sem_create (CLE_SEM_CONSO ,0);
     int shmid = shmget(CLE_MEM,MEM*sizeof(int),0644);
     int* shmem = shmat(shmid,NULL,0);
     for (int i = 0; i < 26; i++)
     {
         P(sem_conso);
         char c = shmem[i%MEM];
         V(sem_prod);
         printf("%c",c);
     }
     printf("\n");
     shmdt(shmem);
     return 0;
}
