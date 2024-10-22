#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

struct args{
    sem_t *canRead;
    sem_t *canWrite;
    int *buffer;
    int *N;
    pthread_mutex_t mutex;
};

void *consoFct(void *arguments){
    struct args *args = arguments;
    int *tab = args->buffer;
    int *N = args->N;
    sem_wait(args->canRead);
    pthread_mutex_lock(&args->mutex);
    int data = tab[tab[*N]];
    int index = tab[*N];
    tab[*N+1] -= 1;
    tab[*N] = (tab[*N]+1)%*N;
    pthread_mutex_unlock(&args->mutex);
    printf("donnée reçue : %d sur index %d\n", data, index);
    sem_post(args->canWrite);

    return NULL;
}

void *prodFct(void *arguments){
    struct args *args = arguments;
    int *tab = args->buffer;
    int *N = args->N;
    int random = rand()%10;
    sleep(random);
    sem_wait(args->canWrite);
    pthread_mutex_lock(&args->mutex);
    tab[(tab[*N] + tab[(*N)+1])%*N] = random;
    int index = (tab[*N] + tab[(*N)+1])%*N;
    tab[(*N)+1] += 1;
    pthread_mutex_unlock(&args->mutex);
    sem_post(args->canRead);
    printf("donnée écrite : %d sur index %d\n", random, index);

    return NULL;
}

int main(int argc, char **argv)
{
    if(argc < 2){
        exit(-1);
    }
    int N = atoi(argv[1]);

    //Initialisation du mutex
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    //semaphore pour savoir si l'écriture est disponible
    sem_t canWrite;
    sem_init(&canWrite,0,N);
    //semaphore pour savoir si la lecture est disponible
    sem_t canRead;
    sem_init(&canRead,0,0);
    //récupère la mémoire partagée (buffer)
    int buffer[N+2];
    //initialise les variables index et qte à 0
    buffer[N] = 0;
    buffer[N+1] = 0;

    int nProd = 12;
    int nConso = 14;
    pthread_t prod[nProd];
    pthread_t conso[nConso];

    struct args args;
    args.canRead = &canRead;
    args.canWrite = &canWrite;
    args.buffer = buffer;
    args.N = &N;
    args.mutex = mutex;

    for(int i = 0; i<nProd; i++){
        pthread_create(&prod[i],NULL,prodFct,&args);
    }
    for(int i = 0; i<nConso; i++){
        pthread_create(&conso[i],NULL,consoFct,&args);
    }
    for(int i = 0; i<nProd; i++){
        pthread_join(prod[i],NULL);
    }
    for(int i = 0; i<nConso; i++){
        pthread_join(conso[i],NULL);
    }
}