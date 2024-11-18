#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main(void)
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler = SIG_IGN;
    sigaction(SIGINT,&act,NULL);
    while(1){
        puts("affiche");
    }
    puts("end");
    return 0;
}
