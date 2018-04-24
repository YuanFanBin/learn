#include "../lib/error.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NLOOP 5000

int counter;    // incremented by threads

void *doit(void *vptr);

int main(int argc, char **argv)
{
    int       n;
    pthread_t tidA, tidB;

    if ((n = pthread_create(&tidA, NULL, &doit, NULL)) < 0) {
        errno = n;
        err_sys("pthread_create error");
    }
    if ((n = pthread_create(&tidB, NULL, &doit, NULL)) < 0) {
        errno = n;
        err_sys("pthread_create error");
    }
    // wait for both threads to terminate
    if ((n = pthread_join(tidA, NULL)) < 0) {
        errno = n;
        err_sys("pthread_join error");
    }
    if ((n = pthread_join(tidB, NULL)) < 0) {
        errno = n;
        err_sys("pthread_join error");
    }
    exit(0);
}

void *doit(void *vptr)
{
    int i, val;
    /*
     * Each thread fetches, prints, and increments the counter NLOOP times.
     * The value of the counter should increase monotonically.
     */

    for (i = 0; i < NLOOP; i++) {
        val = counter;
        printf("%d: %d\n", (int)pthread_self(), val + 1);
        counter = val + 1;
    }
    return(NULL);
}
