#include <stdio.h>
#include "../dijkstra.h"

int main(void){
    sem_create(1,0);
    sem_create(2,0);
    sem_create(3,1);
}
