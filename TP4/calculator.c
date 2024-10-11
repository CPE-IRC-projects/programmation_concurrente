#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc < 2){
        printf("Il faut un nombre de processus à créer.\n");
        exit(-1);
    }

    struct requete_client_serveur {
        int clientPid; //PID du Client
        int nombre1;
        int nombre2;
        char operator;
    };
    struct resultat_client_serveur {
        int nombre1;
        int nombre2;
        int resultat;
        char operator;
    };
    struct requete_client_serveur reqcs;
    struct resultat_client_serveur rescl;
    int mainTube[2];
    pipe(mainTube);
    int nombreIterations = atoi(argv[1]);
    int tubes[nombreIterations][2];
    for(int i = 0; i<nombreIterations;i++){
        pid_t pid = fork();
        pipe(tubes[i]);
        if(pid != 0){
            reqcs.clientPid = pid;
            reqcs.nombre1 = rand()%10;
            reqcs.nombre2 = rand()%10;
            reqcs.operator = rand()%2 == 0 ? '-':'+';
            close(mainTube[0]);
            write(mainTube[1], &reqcs, sizeof(reqcs));
            close(mainTube[1]);
            
            break;            
        }
    }

}