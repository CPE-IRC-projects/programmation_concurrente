#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

//Simple fonction qui monte un float à sa puissance 2
void carre(float *n){
    *n = (*n)*(*n);
}

int main(int argc, char **argv){
    if(argc < 2){
        exit(-1);
    }
    int N = atoi(argv[1]);
    //définit la seed du random sur le time
    srand(time(NULL));
    //définit les deux nombre flotants qui seront utilisés pour les coordonnées
    float x,y;
    //nombre de d'essais qui sont dans le cercle
    int qteHit = 0;

    for(int i = 0; i<N; i++){
        x = ((float)rand()/(float)(RAND_MAX));
        y = ((float)rand()/(float)(RAND_MAX));
        carre(&x);
        carre(&y);
        if(x+y<1){
            qteHit++;
        }
    }
    printf("pi:%f\n",4*((float)qteHit/(float)N));

    return 0;
}