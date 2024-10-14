#include "../dijkstra.h"
#include "data.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
int main()
{
    shmctl(BUFFERKEY, IPC_RMID, NULL);
    shmctl(QTEKEY, IPC_RMID, NULL);
    shmctl(INDEXKEY, IPC_RMID, NULL);
    sem_delete(sem_get(1));
    sem_delete(sem_get(2));
    sem_delete(sem_get(3));
    sem_delete(sem_get(4));
    sem_delete(sem_get(5));
}