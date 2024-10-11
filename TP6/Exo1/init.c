#include <stdio.h>
#include "../dijkstra.h"
#include <sys/ipc.h>
#include <sys/shm.h>
int main(void){
    shmget(5942,sizeof(int),IPC_CREAT|IPC_EXCL|0600);
    sem_create(1,0);
    sem_create(2,1);
}
