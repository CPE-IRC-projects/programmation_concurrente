#include "../dijkstra.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
int main()
{
    shmctl(5942, IPC_RMID, NULL);
    sem_delete(sem_get(1));
    sem_delete(sem_get(2));
}
