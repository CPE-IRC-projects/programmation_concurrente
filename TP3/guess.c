#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
        close(tube[1]);
        close(tube2[0]);
        int random = rand()%50;
        int isFound = 0;
        int response;
        char retour = '=';
        while (isFound == 0){
            read(tube[0], &response, sizeof(int));
            if(response == random){
                isFound = 1;
                retour = '=';
                close(tube[0]);
                close(tube2[1]);
            }else if (response > random){
                retour = '-';
            }else{
                retour = '+';
            }
            write(tube2[1], &retour, sizeof(char));
        }
    }else{
        close(tube[0]);
        close(tube2[1]);
        int number;
        char res;
        while (1)
        {
            printf("Entrez un nombre :\n");
            scanf("%d", &number);
            write(tube[1], &number, sizeof(int));
            read(tube2[0], &res, sizeof(char));
            if(res == '='){
                printf("%d était le nombre à trouver\n", number);
                close(tube[1]);
                close(tube2[0]);
                break;
            }else{
                printf("%c\n", res);
            }
        }
    }
    return 0;
}