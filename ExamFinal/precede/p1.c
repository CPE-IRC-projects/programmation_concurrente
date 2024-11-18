#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "dijkstra.h"
#include "commun.h"

void t1()
{
    puts("t1");
    sleep(10);
    puts("end t1");
}

void t2()
{
    puts("t2");
    sleep(2);
    puts("end t2");
}

int main () {
    //liaison du mutex pour dire quand t1 est fini
    int sem = sem_create (CLE_SEM ,0);
    t1();
    //on libère le mutex
    V(sem);
    t2();
    return 0;
}
