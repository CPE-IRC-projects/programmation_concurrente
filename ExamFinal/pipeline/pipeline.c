#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    int tube[2];
    int res = pipe(tube);
    if(res == -1){
        perror("Création Pipe");
        exit(-1);
    }
    if(fork()==0){
        close(tube[0]);
        int input = open("./server.log",O_RDONLY);
        dup2(input, 0);
        dup2(tube[1], 1);
        close(tube[1]);
        execlp("grep","grep", "invalid credentials", NULL);
    }
    else{
        int tube2[2];
        int res2 = pipe(tube2);
        if(res2 == -1){
            perror("Création Pipe2");
            exit(-1);
        }
        if(fork()==0){
            close(tube[1]);
            close(tube2[0]);
            dup2(tube[0], 0);
            dup2(tube2[1], 1);
            close(tube[0]);
            close(tube2[1]);
            execlp("cut", "cut", "-d,", "-f3", NULL);
        }else{
            close(tube[0]);
            close(tube[1]);
            close(tube2[1]);
            int input = open("./sortie",O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(input, 1);
            dup2(tube2[0], 0);
            close(tube2[0]);
            execlp("sort","sort", NULL);
        }
    }
}