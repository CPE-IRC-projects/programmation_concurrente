#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "dijkstra.h"
#include "commun.h"

void t4()
{
    puts("t4");
    sleep(5);
    puts("end t4");
}

int main () {
    t4();
    return 0;
}
