#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc < 3){
        printf("Il faut au minimum écrire une adresse et une destination");
        return 1;
    }   
    int nbFork = 0;
    pid_t fils;
    for(int i = 1; i< argc-1; i++){
        if(nbFork == 4){
            fils = wait(NULL);
            printf("Programme n°%d est terminé !\n",fils);
            nbFork--;
        }
        if(fork() == 0){
            pid_t moi = getpid();
            printf("Programme n°%d commence le téléchargement !\n",moi);
            int err = execlp("wget", "wget", "-P", argv[argc-1], argv[i],"-q","-o","/dev/null",NULL);
            if(err == -1){
               printf("Erreur lors du téléchargement n°%d \n",moi); 
               nbFork--;
               exit(0);
            }
        }else{
            nbFork++;
        }
    }
    while(nbFork!=0){
        fils = wait(NULL);
        printf("Programme n°%d est terminé !\n",fils);
        nbFork--;
    }

}