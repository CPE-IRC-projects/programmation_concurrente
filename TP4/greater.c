#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    //vérification qu'il y a minimum deux arguments pour le nombre de processus à créer
    if(argc < 2){
        printf("Il faut un nombre de processus à créer.\n");
        exit(-1);
    }
    //création du tube qui fera le lien entre le processus 0 et le processus N
    int finalTube[2];
    int nombreIterations = atoi(argv[1]);
    //définition des différentes variables
    int random;
    int numero;
    pid_t idfork;
    int dfFinalTubeWrite;
    //Boucle pour créer les N processus
    for(int i = 0; i<nombreIterations;i++){
        //variables qui auront les valeurs du processus N-1
        int bestRandom;
        int bestNumero;
        pid_t bestIdFork;
        //création du tube qui communiquera les informations entre n et n+1
        int tube[2];
        int res = pipe(tube);
        
        if(res == -1){
            perror("Création Pipe");
            exit(-1);
        }
        
        //si on est le deuxième processus, il n'y a pas encore de meilleur car il n'y a eu qu'un seul chiffre généré donc c'est forcément lui le meilleur.
        if(i == 1){
            bestRandom = random;
            bestNumero = numero;
            bestIdFork = idfork;
        }
        //Conserve le i
        numero = i;
        //fork et coserve l'id pour vérifier qui est le parent et pour pouvoir donner le pid du fil au processus n+1
        idfork = fork();
        //Si on est le parent
        if(idfork == 0){
            //à chaque itération à part la première, écrit le meilleur de l'itération d'avant.
            if(i > 0){
                close(tube[0]);
                write(tube[1], &bestRandom, sizeof(int));
                write(tube[1], &bestNumero, sizeof(int));
                write(tube[1], &bestIdFork, sizeof(int));
                close(tube[1]);
            }
            //Si on est le processus 0 (parent à la deuxière itération) on attend que le tube final ai une information pour finir le programme.
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
        //On est le fils
        //Si c'est la première itération, définit le tube 
        if(i == 0){
            pipe(finalTube);
            //Entregistre l'entrée du tube pour écrire dedans à l'itération N-1
            dfFinalTubeWrite = finalTube[1];
        }
        //ferme l'entrée du tube fils;
        close(tube[1]);
        //génère le nombre et l'affiche
        random = rand()%3000;
        printf("processus pid %d numéro %d val = %d\n", idfork, i, random);
        //Pour chque itération qui n'est pas la première, lit le meilleur numéro du process n-1 et le nompare avec le sien
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
        //ferme la sortie du tube car on en a plus besoin
        close(tube[0]);
        //Si on est la dernière itération, on doit écrire dans le finaltube (initialisé pendant la première itération) pour fermer la boucle.
        if(i == nombreIterations-1){
            write(dfFinalTubeWrite, &bestRandom, sizeof(int));
            write(dfFinalTubeWrite, &bestNumero, sizeof(int));
            write(dfFinalTubeWrite, &bestIdFork, sizeof(int));
            close(dfFinalTubeWrite);
        }
    }
}