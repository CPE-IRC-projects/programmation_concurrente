#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILTRE "/tmp/filtre"
#define PAIR "/tmp/retpair"
#define IMPAIR "/tmp/retimpair"

// void error(char* file) {
//     fprintf(stderr,"Can't create %s\n",file); exit(1);
// }

int main()
{
    int dff, dfp, dfi;
    dff = open(FILTRE,O_WRONLY);
    
    int number;
    for(int i = 0; i<51; i++){
        if(i==50){
            number = -1;
        }else{
            number = rand()%100;
        }
        if(write(dff, &number, sizeof(int)) != -1){
            printf("%d : Envoyé\n",number);
        }else{
            printf("%d : Erreur d'envoi\n",number);
        }
    }
    dfp = open(PAIR,O_RDONLY);
    dfi = open(IMPAIR,O_RDONLY);
    int sommePair;
    int sommeImpair;

    read(dfp, &sommePair, sizeof(int));
    read(dfi, &sommeImpair, sizeof(int));

    printf("Somme des nombres pairs : %d\nSomme des nombres impairs : %d\nTotal : %d\n", sommePair,sommeImpair,sommeImpair+sommePair);
    close(dff);
    close(dfp);
    close(dfi);

}