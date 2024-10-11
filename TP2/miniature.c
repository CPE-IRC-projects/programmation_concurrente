#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc < 2){
        printf("Il faut le nom du répertoire\n");
        return 0;
    }
    //open directory
    DIR* directory = opendir(argv[1]);
    int nbFork = 0;
    struct dirent* entry;
    while((entry = readdir(directory)) != NULL){
        if(entry->d_type == DT_DIR){
            continue;
        }
        if(nbFork==8){
            wait(NULL);
            nbFork--;
        }
        printf("%s\n", entry->d_name);
        if(fork()==0){
            execlp("convert","convert",strcat(argv[1], entry->d_name),"-resize","10%", argv[1],NULL);
        }else{
            nbFork++;
        }
    }
    //close directory
    closedir(directory);
}