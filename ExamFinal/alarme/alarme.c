#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

pid_t fils;

void redirect(int signum) {
    if (signum == SIGUSR1){
        exit(0);
    }
    else if (signum == SIGALRM){
        kill(fils, SIGUSR1);
    }
}

int main(void)
{
    fils = fork();
    if(fils != 0){
        struct sigaction act;
        memset(&act,0,sizeof(act));
        act.sa_handler = redirect;
        sigaction(SIGALRM,&act,NULL);
        alarm(3);
        pause();
    }
    else{
        struct sigaction act;
        act.sa_handler = redirect;
        memset(&act,0,sizeof(act));
        sigaction(SIGUSR1,&act,NULL);
        while(1){
            puts("fils");
            sleep(1);
        }
    }
    return 0;
}