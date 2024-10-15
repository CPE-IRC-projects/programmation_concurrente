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
    int mutexwrite = sem_get(3);
    int mutexread = sem_get(5);

    //récupère la mémoire partagée (table)
    int shmidtable = shmget(TABLEKEY,TABLESIZE*sizeof(int),0);
    int* table = shmat(shmidtable,NULL,0);
    //initialisation du tableau avec 3 données (peut être fait avec N données et des données aléatoires)
    int tableLocale[3] = {4,69,32};
    int min = 100;
    int indexMin;
    printf("base : %d,%d,%d\n",tableLocale[0],tableLocale[1],tableLocale[2]);
    
    while(1){
        //recherche du plus petit élément
        for(int i = 0; i<3; i++){
            if(tableLocale[i]<min){
                min = tableLocale[i];
                indexMin = i;
            }
        }
        //On prend possesion de la mémoire partagée pour mettre notre élément le plus grand
        P(mutextable);
        table[1] = min;
        printf("dépot de %d dans table[1]\n",min);
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
        tableLocale[indexMin] = table[1];
        min = tableLocale[indexMin];
        V(mutextable);

    }
    //on détache la mémoire et affiche le resultat
    shmdt(table);
    printf("max : %d,%d,%d\n",tableLocale[0],tableLocale[1],tableLocale[2]);
}