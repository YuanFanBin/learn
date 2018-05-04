#include "../lib/error.h"
#include "pthread07.h"
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
    int              connfd;
    void             web_child(int);
    socklen_t        clilen;
    struct sockaddr *cliaddr;

    cliaddr = (struct sockaddr *) malloc(addrlen);

    printf("thread %d starting\n", (int) arg);
    for ( ; ; ) {
        clilen = addrlen;
        pthread_mutex_lock(&mlock);
        if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
            pthread_mutex_unlock(&mlock);
            err_sys("accept error");
        }
        pthread_mutex_unlock(&mlock);
        tptr[(int)arg].thread_count++;

        web_child(connfd); // process request
        close(connfd);
    }
}
