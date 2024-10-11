#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    int n = 0;
    if (argc != 2)
        exit(-1);
    int success = sscanf(argv[1],"%d",&n);
    if(!success)
        exit(-2);
    for(int i=0; i<n;i++)
    {
        pid_t fils = fork();
        pid_t pere = getppid();
        pid_t moi = getpid();
        printf("%d - Je suis %d, mon père est %d, fork à retourner %d\n",i,moi,pere,fils);
    }
    return 0;
}