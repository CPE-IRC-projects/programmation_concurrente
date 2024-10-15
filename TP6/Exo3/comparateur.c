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
    int mutexwriteMax = sem_get(2);
    int mutexwriteMin = sem_get(3);
    int mutexreadMax = sem_get(4);
    int mutexreadMin = sem_get(5);

    //récupère la mémoire partagée (table)
    int shmidtable = shmget(TABLEKEY,TABLESIZE*sizeof(int),0);
    int* table = shmat(shmidtable,NULL,0);
    //force l'initialisation de la troisième valeur du tableau à quelque chose de différent de -1
    P(mutextable);
    table[2] = 0;
    V(mutextable);

    while(1){
        //Attend que les deux aient finis d'écrire (peut être fait avec un seul sémaphore mais je trouve ça plus propre)
        P(mutexwriteMax);
        P(mutexwriteMin);
        //Prend possesion de la mémoire partagée
        P(mutextable);
        //Sort de la boucle si les valeurs sont bonnes
        if(table[0] < table[1]){
            table[2] = -1;
            V(mutextable);
            V(mutexreadMax);
            V(mutexreadMin);    
            break;
        }
        //Inversion des deux valeurs du tableau (sans 3e variable pour le flex)
        table[0] += table[1];
        table[1] = table[0] - table[1];
        table[0] = table[0] - table[1];
        //Libère les sémaphores
        V(mutextable);
        V(mutexreadMin);
        V(mutexreadMax);

    }
    shmdt(table);
}