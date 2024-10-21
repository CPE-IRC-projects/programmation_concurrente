#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <pthread.h>
#include <semaphore.h>

struct args
{
    int n;
    int *waiters;
    int total;
    pthread_mutex_t mutex;
    sem_t *sem;
};


void *rdv(void* arg){
    struct args *argument = arg;
    int time2wait = rand()%10;
    printf("%d dors %ds\n", argument->n, time2wait);
    sleep(time2wait);
    printf("%d attend\n", argument->n);
    pthread_mutex_lock(&argument->mutex);
    *argument->waiters = *argument->waiters+1;

    if(*argument->waiters == argument->total){
        for(int i=0; i<argument->total-1;i++){
            sem_post(argument->sem);
        }
    }else{
        sem_wait(argument->sem);
    }
    pthread_mutex_unlock(&argument->mutex);
    
    printf("%d wakeup\n", argument->n);
    return NULL;
}

int main(int argc, char **argv){
    if(argc < 2){
        exit(-1);
    }
    int N = atoi(argv[1]);
    int attente = 0;
    pthread_t thread[N];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    sem_t semaphore;
    sem_init(&semaphore,0,0);
    struct args arguments[N];


    for(int i = 0; i<N; i++){
        arguments[i].n = i;
        arguments[i].waiters = &attente;
        arguments[i].total = N;
        arguments[i].mutex = mutex;
        arguments[i].sem = &semaphore;
        pthread_create(&thread[i],NULL,rdv,&arguments[i]);
    }

    for(int i = 0; i<N; i++){
        pthread_join(thread[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);
    return 0;
}