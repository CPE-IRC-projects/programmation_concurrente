#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc < 2){
        printf("Il faut un nombre de processus à créer.\n");
        exit(-1);
    }
    int finalTube[2];
    int nombreIterations = atoi(argv[1]);
    int random;
    int numero;
    pid_t idfork;
    int dfFinalTubeWrite;
    for(int i = 0; i<nombreIterations;i++){
        int bestRandom;
        int bestNumero;
        pid_t bestIdFork;

        int tube[2];
        int res = pipe(tube);
        
        if(res == -1){
            perror("Création Pipe");
            exit(-1);
        }
        
        if(i == 1){
            bestRandom = random;
            bestNumero = numero;
            bestIdFork = idfork;
        }
        numero = i;
        idfork = fork();
        if(idfork == 0){
            if(i > 0){
                close(tube[0]);
                write(tube[1], &bestRandom, sizeof(int));
                write(tube[1], &bestNumero, sizeof(int));
                write(tube[1], &bestIdFork, sizeof(int));
                close(tube[1]);
            }
            if(i == 1){
                // read sur un truc
                close(finalTube[1]);
                read(finalTube[0],&bestRandom, sizeof(int));
                read(finalTube[0],&bestNumero, sizeof(int));
                read(finalTube[0],&bestIdFork, sizeof(int));
                close(finalTube[0]);
                printf("Le plus grand nombre = %d - pid = %d - Numéro %d\n", bestRandom, bestIdFork, bestNumero);
            }
            break;
        }
        if(i == 0){
            pipe(finalTube);
            dfFinalTubeWrite = finalTube[1];
        }
        close(tube[1]);
        random = rand()%3000;
        printf("processus pid %d numéro %d val = %d\n", idfork, i, random);
        if(i>0){
            read(tube[0],&bestRandom, sizeof(int));
            if(bestRandom > random){
                read(tube[0],&bestNumero, sizeof(int));
                read(tube[0],&bestIdFork, sizeof(int));
            }else{
                bestRandom = random;
                bestNumero = numero;
                bestIdFork = idfork;
            }
        }
        close(tube[0]);
        if(i == nombreIterations-1){
            write(dfFinalTubeWrite, &bestRandom, sizeof(int));
            write(dfFinalTubeWrite, &bestNumero, sizeof(int));
            write(dfFinalTubeWrite, &bestIdFork, sizeof(int));
            close(dfFinalTubeWrite);
        }
    }
}