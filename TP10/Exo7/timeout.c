#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void boucle(){
    alarm(5);
    puts("Svp un entier : ");
    char c[100];
    fgets(c, sizeof(c), stdin);
    int value;
    int reussi = sscanf(c, "%d", &value);
    if(reussi)
}

void redirect(int signum) {
    if (signum == SIGALRM){
        boucle();
    }
}

int main(void)
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler = redirect;
    sigaction(SIGALRM,&act,NULL);
    puts("Entrez un entier en moins de 5 secondes");
    boucle();

    return 0;
}
