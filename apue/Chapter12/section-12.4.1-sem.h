/* sem.h */
#ifndef _SEM_H_
#define _SEM_H_
#include <pthread.h>
struct semaphore {
    pthread_mutex_t lock;
    pthread_cond_t  nonzero;
    unsigned count;
};
typedef struct semaphore semaphore_t;

semaphore_t *semaphore_create(char *semaphore_name);
semaphore_t *semaphore_open(char *semaphore_name);
void semaphore_post(semaphore_t *semap);
void semaphore_wait(semaphore_t *semap);
void semaphore_close(semaphore_t *semap);
#endif

/* man pthread_mutex_attr_init */

/* 
 * 实现一个信号量机制。
 */
