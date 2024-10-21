#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct args
{
    int n;
    pthread_barrier_t* barrier;
};


void *rdv(void* arg){
    struct args *argument = arg;
    int time2wait = rand()%10;
    printf("%d dors %ds\n", argument->n, time2wait);
    sleep(time2wait);
    printf("%d Attend\n", argument->n);
    pthread_barrier_wait(argument->barrier);
    printf("%d wakeup\n", argument->n);
    return NULL;
}

int main(int argc, char **argv){
    if(argc < 2){
        exit(-1);
    }
    int N = atoi(argv[1]);
    pthread_t thread[N];
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier,NULL, N);
    struct args arguments[N];
    for(int i = 0; i<N; i++){
        arguments[i].n = i;
        arguments[i].barrier = &barrier;
        pthread_create(&thread[i],NULL,rdv,&arguments[i]);
    }

    for(int i = 0; i<N; i++){
        pthread_join(thread[i], NULL);
    }
    pthread_barrier_destroy(&barrier);

    return 0;
}