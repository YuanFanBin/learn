#include "../lib/error.h"
#include "pthread08.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void thread_make(int i)
{
    int e;
    void *thread_main(void *);

    if ((e = pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *) i)) != 0) {
        errno = e;
        err_sys("pthread_create error");
    }
    // main thread returns
}

void *thread_main(void *arg)
{
    int  connfd;
    void web_child(int);

    printf("thread %d starting\n", (int) arg);
    for ( ; ; ) {
        pthread_mutex_lock(&clifd_mutex);
        while (iget == iput) {
            pthread_cond_wait(&clifd_cond, &clifd_mutex);
        }
        connfd = clifd[iget];   // connected socket to service
        if (++iget == MAXNCLI) {
            iget = 0;
        }
        pthread_mutex_unlock(&clifd_mutex);
        tptr[(int) arg].thread_count++;

        web_child(connfd);  // process request
        close(connfd);
    }
}
