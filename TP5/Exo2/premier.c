#include "../dijkstra.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int semid = sem_get(1);
    int semid2 = sem_get(2);
    int semid3 = sem_get(3);
    int random = (rand()%10)+1;
    printf("%d\n",random);
    sleep(random);
    printf("premier attend\n");
    V(semid2);
    V(semid2);
    P(semid);
    P(semid3);
    printf("premier fini\n");
}
