#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void redirect(int signum) {
    if (signum == SIGINT){
        printf("SIGPIPE(%d) signal received\n",signum);
        exit(0);
    }
}

int main(void)
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler = redirect;
    sigaction(SIGINT,&act,NULL);
    while(1){
        puts("affiche");
    }
    puts("end");
    return 0;
}
