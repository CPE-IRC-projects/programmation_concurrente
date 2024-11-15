#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


int N;
int nbThreads;
//nombre de d'essais qui sont dans le cercle
int qteHit = 0;

void *calc(void *args){
    //récupère le mutex
    pthread_mutex_t *mutex = args;
    //static initialise la variable une seule fois, elle est partagée ensuite entre tous les threads.
    //
    static int th_id = 0;
    pthread_mutex_lock(mutex);
    th_id++;
    pthread_mutex_unlock(mutex);
    int hit = 0;
    //définit les deux nombre flotants qui seront utilisés pour les coordonnées
    float x,y;
    int it = N/nbThreads;
    uint seed = getpid() + th_id;
    for(int i = 0; i<it; i++){
        x = ((float)rand_r(&seed)/(float)(RAND_MAX));
        y = ((float)rand_r(&seed)/(float)(RAND_MAX));
        if(x*x+y*y<1){
            hit++;
        }
    }
    //Ajoute le compte local au compte global
    pthread_mutex_lock(mutex);
    qteHit += hit;
    pthread_mutex_unlock(mutex);

    return NULL;
}

int main(int argc, char **argv){
    if(argc < 3){
        exit(-1);
    }
    N = atoi(argv[1]);
    nbThreads = atoi(argv[2]);
    //création des threads
    pthread_t thread[nbThreads];
    //création du mutex
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i<nbThreads;i++){
        pthread_create(&thread[i],NULL,calc,&mutex);
    }
    for(int i = 0; i<nbThreads;i++){
        pthread_join(thread[i], NULL);
    }
    printf("pi:%f\n",4*((float)qteHit/(float)N));

    return 0;
}