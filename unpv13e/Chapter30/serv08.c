#include "../lib/error.h"
#include "pthread08.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int nthreads;
pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  clifd_cond = PTHREAD_COND_INITIALIZER;

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int main(int argc, char **argv)
{
    int              i, listenfd, connfd;
    void             sig_int(int), thread_make(int);
    socklen_t        addrlen, clilen;
    struct sockaddr *cliaddr;

    if (argc == 3) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 4) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: serv08 [ <host> ] <port#> <#threads>");
    }
    nthreads = atoi(argv[argc - 1]);
    tptr = (Thread *) calloc(nthreads, sizeof(Thread));
    iget = iput = 0;

    // create all the threads
    for (i = 0; i < nthreads; i++) {
        thread_make(i); // only main thread returns
    }

    signal(SIGINT, sig_int);

    for ( ; ; ) {
        clilen = addrlen;
        if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
            err_sys("accept error");
        }

        pthread_mutex_lock(&clifd_mutex);
        clifd[iput] = connfd;
        if (++iput == MAXNCLI) {
            iput = 0;
        }
        if (iput == iget) {
            err_quit("iput = iget = %d", iput);
        }
        pthread_cond_signal(&clifd_cond);
        pthread_mutex_unlock(&clifd_mutex);
    }
}

void sig_int(int signo)
{
    int  i;
    void pr_cpu_time(void);

    pr_cpu_time();

    for (i = 0; i < nthreads; i++) {
        printf("thread %d, %ld connections\n", i, tptr[i].thread_count);
    }
    exit(0);
}
