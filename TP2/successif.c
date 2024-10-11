#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
    if(fork() == 0){
        execlp("du","du","-sh","/usr/share",NULL);
    }
    wait(NULL);
    if(fork() == 0){
        execlp("ls","ls","-l",NULL);
    }
    wait(NULL);
    if(fork() == 0){
        execlp("ps","ps",NULL);
    }
    wait(NULL);
}