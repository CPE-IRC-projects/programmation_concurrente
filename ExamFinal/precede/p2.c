#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "dijkstra.h"
#include "commun.h"

void t3()
{
    puts("t3");
    sleep(7);
    puts("end t3");
}

int main () {
    t3();
    return 0;
}
