#include "../dijkstra.h"
#include "data.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    //récupère les mutexs
    int mutextable = sem_get(1);
    int mutexwrite = sem_get(2);
    int mutexread = sem_get(4);

    //récupère la mémoire partagée (table)
    int shmidtable = shmget(TABLEKEY,TABLESIZE*sizeof(int),0);
    int* table = shmat(shmidtable,NULL,0);
    //initialisation du tableau avec 3 données (peut être fait avec N données et des données aléatoires)
    int tableLocale[3] = {15,89,3};
    int max = 0;
    int indexMax;
    printf("base : %d,%d,%d\n",tableLocale[0],tableLocale[1],tableLocale[2]);
    
    while(1){
        //recherche du plus grand élément
        for(int i = 0; i<3; i++){
            if(tableLocale[i]>max){
                max = tableLocale[i];
                indexMax = i;
            }
        }
        //On prend possesion de la mémoire partagée pour mettre notre élément le plus grand
        P(mutextable);
        table[0] = max;
        printf("dépot de %d dans table[0]\n",max);
        V(mutextable);
        //On annonce qu'on a fini décrire pour réveiller le comparateur
        V(mutexwrite);
        //On attend que le comparateur ait comparé
        P(mutexread);
        P(mutextable);
        //On sait qu'on a plus rien à faire donc libère la table et sort de la boucle
        if(table[2] == -1){
            V(mutextable);
            break;
        }
        //insertion de la nouvelle valeur et réatribution de max
        tableLocale[indexMax] = table[0];
        max = tableLocale[indexMax];
        V(mutextable);

    }
    //on détache la mémoire et affiche le resultat
    shmdt(table);
    printf("min : %d,%d,%d\n",tableLocale[0],tableLocale[1],tableLocale[2]);
}