#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include "dijkstra.h"
#include "commun.h"

int main () {
    //Erreur ici (SEM doit être remplacé par MEM)
    int sem_prod = sem_create (CLE_SEM_PROD ,MEM);
    int sem_conso = sem_create (CLE_SEM_CONSO ,0);
    int shmid = shmget(CLE_MEM,MEM*sizeof(int),IPC_CREAT|IPC_EXCL|0644);
    if (shmid == -1)
        shmid = shmget(CLE_MEM,MEM*sizeof(int),0644);
    int* shmem = shmat(shmid,NULL,0);
    memset(shmem,0,MEM*sizeof(int));
    if (fork() == 0)
    {
        puts("consommateur");
        execlp("./conso","./conso",NULL);
    }
    if (fork() == 0)
    {
        puts("producteur");
        execlp("./prod","./prod",NULL);
    }
    wait(NULL);
    wait(NULL);
    shmdt(shmem);
    shmctl(shmid,IPC_RMID,NULL);
    sem_delete(sem_prod);
    sem_delete(sem_conso);
    return 0;
}
