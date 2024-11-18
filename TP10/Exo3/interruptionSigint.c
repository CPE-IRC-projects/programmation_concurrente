#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int fin = 0;

void redirect(int signum) {
    if (signum == SIGINT){
        fin = 1;
    }
}

int main(void)
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler = redirect;
    sigaction(SIGINT,&act,NULL);
    while(fin == 0){
        puts("affiche");
    }
    puts("end");
    return 0;
}
