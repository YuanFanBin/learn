/* post */
#include<pthread.h>
#include<stdlib.h>
#include "section-12.4.1-sem.h"

/* gcc section-12.4.1-sem.h section-12.4.1-sem.c section-12.4.1-post.c -lpthread -o post */
int
main(int argc, char *argv[])
{
    semaphore_t *semap;

    semap = semaphore_open("/tmp/semaphore");
    if (semap == NULL)
        exit(1);
    semaphore_post(semap);
    semaphore_close(semap);
    return(0);
}

/* man pthread_mutex_attr_init */

/* 
 * 实现一个信号量机制。
 */
