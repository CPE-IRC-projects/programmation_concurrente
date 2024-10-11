#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    int tube[2];
    int tube2[2];
    int res = pipe(tube);
    int res2 = pipe(tube2);
    if(res == -1){
        perror("Création Pipe");
        exit(-1);
    }
    if(res2 == -1){
        perror("Création Pipe2");
        exit(-1);
    }
    if(fork()==0){
        close(tube2[0]);
        close(tube2[1]);
        close(tube[0]);
        int input = open("/etc/passwd",O_RDONLY);
        dup2(input, 0);
        close(tube[1]);
        execlp("cut","cut", "-f", "1,3", "-d", ":", NULL);
    }else{
        if(fork()==0){
            close(tube[1]);
            close(tube2[0]);
            dup2(tube[0], 0);
            dup2(tube2[1], 1);
            close(tube[0]);
            close(tube2[1]);
            execlp("sed", "sed", "s+^\\(.\\):\\(.*\\)+\2:\1+", NULL);
        }else{
            close(tube[0]);
            close(tube[1]);
            close(tube2[1]);
            int input = open("./users",O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(input, 1);
            dup2(tube2[0], 0);
            close(tube2[0]);
            execlp("sort","sort", "-n", NULL);
        }
    }
}