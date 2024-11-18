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
    if(reussi==1){
        alarm(0);
        puts("Ok merci !!");
    }else{
        boucle();
    }
}

void redirect(int signum) {
    if (signum == SIGALRM){
        puts("Trop tard !!");
        exit(0);
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
