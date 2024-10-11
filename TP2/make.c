#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char **argv)
{
    char *args[argc+4];
    args[0] = "gcc";
    args[1] = "-Wall";
    args[2] = "-Wextra";
    for(int i = 1; i<argc-1;i++){
        if(fork()==0){
            size_t size = strlen(argv[i]);
            char *temp = strdup(argv[i]);
            temp[size-1] = 'o';
            execlp("gcc","gcc","-Wall","-Wextra","-c", argv[i], "-o", temp,NULL);
        }else{
            size_t size = strlen(argv[i]);
            char *temp = strdup(argv[i]);
            temp[size-1] = 'o';
            args[i+2] = temp; 
        }
    }
    for(int i = 1; i< argc;i++){
        wait(NULL);
    }
    args[argc+1] = "-o";
    args[argc+2] = argv[argc-1];
    args[argc+3] = NULL;

    if(fork()==0){
        execvp("gcc",args);
    }
    wait(NULL);
}