#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PAIR "/tmp/pair"
#define IMPAIR "/tmp/impair"
#define RETPAIR "/tmp/retpair"
#define RETIMPAIR "/tmp/retimpair"


int main()
{
    int dfr, df;
    int number;
    int somme = 0;

    if(fork()==0){
        unlink(PAIR);
        unlink(RETPAIR);
        if(mkfifo(PAIR,0644) == -1){ exit(-1); }
        if(mkfifo(RETPAIR,0644) == -1){ exit(-1); }
        df = open(PAIR,O_RDONLY);
        dfr = open(RETPAIR,O_WRONLY);
    }else{
        unlink(IMPAIR);
        unlink(RETIMPAIR);
        if(mkfifo(IMPAIR,0644) == -1){ exit(-1); }
        if(mkfifo(RETIMPAIR,0644) == -1){ exit(-1); }
        df = open(IMPAIR,O_RDONLY);
        dfr = open(RETIMPAIR,O_WRONLY);
    }

    while(1){
        read(df, &number, sizeof(int));
        if(number == -1){
            write(dfr, &somme, sizeof(int));
            break;
        }
        somme += number;
    }
}