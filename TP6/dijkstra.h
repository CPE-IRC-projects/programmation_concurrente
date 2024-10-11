#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <sys/sem.h>

/**
 * return the semaphore associated to the key cle
 * create the semaphore with initval token then return it
 */
int sem_create(key_t cle, int initval);

/**
 *  return the semaphore associated to the key cle
 *  exit if not exist
 */
int sem_get(key_t cle);

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
