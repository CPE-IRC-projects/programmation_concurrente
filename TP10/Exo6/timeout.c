#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void redirect(int signum) {
    if (signum == SIGUSR1){
        puts("interception SIGUSR1");
    }
}

int main(void)
{
    pid_t fils = fork();
    if(fils != 0){
        for(int i=1; i<7; i++){
            if(i == 3 || i == 5){
                kill(fils, SIGUSR1);
            }
            puts("père");
            sleep(1);
        }
        kill(fils, SIGUSR2);
    }else{

        struct sigaction act;
        memset(&act,0,sizeof(act));
        act.sa_handler = redirect;
        sigaction(SIGUSR1,&act,NULL);
        while(1){
            puts("fils");
            sleep(1);
        }
    }
    return 0;
}