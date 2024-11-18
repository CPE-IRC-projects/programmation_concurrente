#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void redirect(int signum) {
    if (signum == SIGINT){
        puts("interception SIGINT");
    }
}

int main(void)
{
    pid_t fils = fork();
    if(fils != 0){
        for(int i=0; i<5; i++){
            puts("père");
            sleep(1);
        }
    }else{

        struct sigaction act;
        memset(&act,0,sizeof(act));
        act.sa_handler = redirect;
        sigaction(SIGINT,&act,NULL);
        while(1){
            puts("fils");
            sleep(1);
        }
    }
    return 0;
}
