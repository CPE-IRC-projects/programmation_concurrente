#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include "dijkstra.h"
#include "commun.h"

int main () {
    //création d'un mutex pour lancer t3 après t1
    int sem = sem_create (CLE_SEM ,0);
    if (fork() == 0)
    {
        execlp("./p1","./p1",NULL);
    }
    //on attend sur le mutex
    P(sem);
    if (fork() == 0)
    {
        execlp("./p2","./p2",NULL);
    }
    //wait les deux programmes précédents pour lancer t4 à la fin du reste
    wait(NULL);
    wait(NULL);
    if (fork() == 0)
    {
        execlp("./p3","./p3",NULL);
    }
    wait(NULL);
    return 0;
}
