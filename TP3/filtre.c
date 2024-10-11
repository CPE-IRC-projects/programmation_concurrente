#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILTRE "/tmp/filtre"
#define PAIR "/tmp/pair"
#define IMPAIR "/tmp/impair"

void error(char* file) {
    fprintf(stderr,"Can't create %s\n",file); exit(1);
}

int main()
{
    int dff, dfp, dfi;
    unlink(FILTRE);
    if(mkfifo(FILTRE,0644) == -1){ error(FILTRE); }
    printf("En attente de données\n");
    dff = open(FILTRE,O_RDONLY);
    dfp = open(PAIR,O_WRONLY);
    dfi = open(IMPAIR,O_WRONLY);
    int number;
    while(1){
        read(dff, &number, sizeof(int));
        printf("%d\n",number);
        if(number == -1){
            write(dfp, &number, sizeof(int));
            write(dfi, &number, sizeof(int));
            break;
        }
        if(number % 2 == 0){
            printf("pair\n");
            write(dfp, &number, sizeof(int));
        }else{
            printf("impair\n");
            write(dfi, &number, sizeof(int));
        }
    }
    close(dff);
    close(dfp);
    close(dfi);
}