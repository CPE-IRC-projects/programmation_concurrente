#include "../dijkstra.h"
#include "data.h"

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int main(void){
    shmget(BUFFERKEY,BUFFERSIZE*sizeof(int),IPC_CREAT|IPC_EXCL|0600);
    shmget(BUFFERQTEKEY, sizeof(int),IPC_CREAT|IPC_EXCL|0600);
    shmget(READWRITEKEY, sizeof(int),IPC_CREAT|IPC_EXCL|0600);
    sem_create(1,1);
    sem_create(2,1);
    sem_create(3,1);
}
