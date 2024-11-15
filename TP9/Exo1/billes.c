#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int N;
int nb_max_billes;
int nb_billes_dispo;
int m;
pthread_mutex_t mutex;
sem_t semaphore;
int canEnd = 0;

void *controller(void *){
    while(canEnd == 0){
        pthread_mutex_lock(&mutex);
        //vérifie que le nombre de billes soit valide
        if(0 > nb_billes_dispo || nb_max_billes < nb_billes_dispo){
            puts("erreur :/");
            printf("%d\n", nb_billes_dispo);
            exit(-1);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void demander(int nBilles){
    //lock pour le while
    pthread_mutex_lock(&mutex);
    while(nBilles>nb_billes_dispo){
        //unlock pour avant le while et pour celui à la fin lorsque ça boucle
        pthread_mutex_unlock(&mutex);
        //attend qu'il y ai des billes qui soit rendues
        sem_wait(&semaphore);
        pthread_mutex_lock(&mutex);
    }
    nb_billes_dispo -= nBilles;
    pthread_mutex_unlock(&mutex);
}

void rendre(int nBilles){
    pthread_mutex_lock(&mutex);
    //rend n billes
    nb_billes_dispo += nBilles;
    pthread_mutex_unlock(&mutex);
    //rend N au sémaphore pour réveiller tous les threads
    for(int i=0;i<N;i++){
        sem_post(&semaphore);
    }

}

//fonction appelée par tous les threads
void *travailleur(void *args){
    //récupère la valeur qu'on a mis dans le tableau au moment de l'initialisation des valeurs
    int *nb_bille_necessaires = args;
    for(int i=0; i<m;i++){
        //demande de travailler avec n billes
        demander(*nb_bille_necessaires);
        printf("travaille avec %d billes\n", *nb_bille_necessaires);
        //travaille
        sleep((rand()%10)+1);
        printf("rend %d billes\n", *nb_bille_necessaires);
        //rend les n billes
        rendre(*nb_bille_necessaires);
    }
    return NULL;
}


int main(int argc, char **argv){
    if(argc < 4){
        printf("./billes <nombre de threads> <nombre de billes max> <nombre d'itération pour chaque thread>\n");
        exit(-1);
    }
    //récupère les 3 arguments
    N = atoi(argv[1]);
    nb_max_billes = atoi(argv[2]);
    m = atoi(argv[3]);
    nb_billes_dispo = nb_max_billes;
    //tableau pour les k billes nécéssaires à chaque thread
    int billesNecessaires[N];
    //change le seed du random
    srand(time(NULL));
    //définit le nombre de billes nécéssaires pour chaque thread.
    for(int i = 0; i<N; i++){
        billesNecessaires[i] = (rand()%nb_max_billes)+1;
    }
    //création des threads
    pthread_t thread[N+1];
    //création du mutex
    pthread_mutex_init(&mutex, NULL);
    //Initialise le sémaphore qui sera utilisé pour réveiller tous les threads à chaque passage dans la fonction rendre()
    sem_init(&semaphore,0,N);
    //créé tous les threads (hors controlleur)
    for(int i = 0; i<N;i++){
        pthread_create(&thread[i],NULL,travailleur,&billesNecessaires[i]);
    }
    //créé le controlleur
    pthread_create(&thread[N],NULL,controller,NULL);
    //attend tous les threads (hors controlleur)
    for(int i = 0; i<N;i++){
        pthread_join(thread[i], NULL);
    }
    //dit au controlleur que tous les threads sont finis
    canEnd=1;
    pthread_join(thread[N], NULL);
    return 0;
}