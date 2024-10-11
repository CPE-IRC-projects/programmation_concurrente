#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if(argc < 2){
        return 1;
    }
    int N;
    pid_t fils;
    int nb_bon = sscanf(argv[1],"%d",&N);
    if(nb_bon<1){
        return 2;
    }
    for(int i = 0; i<N; i++){
        fils = fork();
        if(fils == 0){
            pid_t pere = getppid();
            pid_t moi = getpid();
            printf("Je suis %d, mon père est %d\n",moi,pere);
            sleep(2*i);
            exit(i);
        }
    }
    if(fils > 0){
        for(int i = 0; i<N; i++){
            int status;
            fils = wait(&status);
            printf("Fils terminé : %d, raison : %d\n", fils, status);
        }
    }
}