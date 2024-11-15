#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


int N;
//nombre de d'essais qui sont dans le cercle
int qteHit = 0;
int nbThreads = 4;

//Simple fonction qui monte un float à sa puissance 2
float carre(float n){
    return n*n;
}

// struct args
// {
//     // pthread_mutex_t mutex;
// };

void *calc(void *args){
    pthread_mutex_t *mutex = args;
    int hit = 0;
    //définit les deux nombre flotants qui seront utilisés pour les coordonnées
    float x,y;
    int it = N/nbThreads;
    for(int i = 0; i<it; i++){
        x = ((float)rand()/(float)(RAND_MAX));
        y = ((float)rand()/(float)(RAND_MAX));
        //x=carre(x);
        //y=carre(y);
        if(x*x+y*y<1){
            hit++;
        }
    }
    puts("end calc");
    pthread_mutex_lock(mutex);
    qteHit += hit;
    pthread_mutex_unlock(mutex);
    puts("end");

    return NULL;
}

int main(int argc, char **argv){
    if(argc < 2){
        exit(-1);
    }
    N = atoi(argv[1]);
    //définit la seed du random sur le time
    srand(time(NULL));
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