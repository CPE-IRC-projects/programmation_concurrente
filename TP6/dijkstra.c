#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

#include "dijkstra.h"

int sem_create(key_t cle, int initval) {
	int semid;
	union semun { int val; struct semid_ds *buf; ushort *array; } arg_ctl;

	semid = semget(cle, 1 , IPC_CREAT|IPC_EXCL|0600);
	if (semid == -1) 
	{
        return -1;
    } 
	else 
	{ 
		arg_ctl.val = initval;
		if (semctl(semid, 0, SETVAL, arg_ctl) == -1) 
		{
			perror("Erreur initialisation sémaphore"); 
			exit(1);
		}
	}
	return(semid) ;
}

int sem_get(key_t cle)
{

	int semid = semget(cle,1, 0600);
    if (semid == -1) 
    {
        perror("Erreur semget()"); 
        exit(1);
    }
    return semid;
}

void P(int semid) 
{
	struct sembuf sempar;
	sempar.sem_num = 0;
	sempar.sem_op = -1;
	sempar.sem_flg = 0;
	if (semop(semid, &sempar, 1) == -1) 
		perror("Erreur operation P");
}

void V(int semid) 
{
	struct sembuf sempar;
	sempar.sem_num = 0;
	sempar.sem_op = 1;
	sempar.sem_flg = 0;
	if (semop(semid, &sempar, 1) == -1) 
		perror("Erreur opération V");
}

void sem_delete(int semid) 
{
	if (semctl(semid,0,IPC_RMID,0) == -1) 
		perror("Erreur dans destruction sémaphore");
}
