#include "../dijkstra.h"
#include "data.h"

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int main(void){
    sem_create(1,1);
    sem_create(2,0);
    sem_create(3,0);
    sem_create(4,0);
    sem_create(5,0);
    shmget(TABLEKEY,TABLESIZE*sizeof(int),IPC_CREAT|IPC_EXCL|0600);
}
