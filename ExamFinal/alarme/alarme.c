#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

pid_t fils;
//fonction qui gère les signaux
void redirect(int signum) {
    if (signum == SIGUSR1){
        exit(0);
    }
    else if (signum == SIGALRM){
        //envoie le signal SIGUSR1 au fils
        kill(fils, SIGUSR1);
    }
}

int main(void)
{
    //définit quoi faire lors d'un signal
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler = redirect;
    fils = fork();
    if(fils != 0){
        //écoute le signal SIGALRM
        sigaction(SIGALRM,&act,NULL);
        //set l'alarme à 10s
        alarm(10);
        //pause et en pause le programme jusqu'à une interruption
        pause();
    }
    else{
        //écoute le signal SIGUSR1
        sigaction(SIGUSR1,&act,NULL);
        while(1){
            puts("fils");
            sleep(1);
        }
    }
    return 0;
}