#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
    int n = 0;
    for(int i = 0; i<4;i++){
        pid_t fils = fork();
        if(fils == 0 ){
            for(n = (50*i)+1; n<=50*(i+1);n++){
                printf("%d\n",n);
            }
            exit(0);
        }
        fils = wait(NULL);
    }
    return 0;
}