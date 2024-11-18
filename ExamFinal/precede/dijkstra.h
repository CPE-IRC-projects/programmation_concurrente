#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <sys/sem.h>

/**
 * return the semaphore associated to the key cle
 * if the semaphore doesn't already exist the function create it with initval token then return it
 */
int sem_create(key_t cle, int initval);

/**
 * take a token on the semaphore semid or wait if none available
 */
void P(int semid);

/**
 * put a token on the semaphore semid
 */
void V(int semid);

/**
 * delete the semaphore semid
 */
void sem_delete(int semid);

#endif
