#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main(void)
{
    pid_t fils = fork();
    if(fils != 0){
        for(int i=0; i<5; i++){
            if(i==3){
                kill(fils,SIGKILL);
            }
            puts("père");
            sleep(1);
        }
    }else{
        while(1){
            puts("fils");
            sleep(1);
        }
    }
    return 0;
}
