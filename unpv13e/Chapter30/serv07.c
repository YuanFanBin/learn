#include "../lib/error.h"
#include "pthread07.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int main(int argc, char **argv)
{
    int  i;
    void sig_int(int), thread_make(int);

    if (argc == 3) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 4) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: serv07 [ <host> ] <port#> <#threads>");
    }
    nthreads = atoi(argv[argc - 1]);
    tptr = (Thread *) calloc(nthreads, sizeof(Thread));
    for (i = 0; i < nthreads; i++) {
        thread_make(i); // only main thread returns
    }

    signal(SIGINT, sig_int);

    for ( ; ; ) {
        pause();    // everything done by threads
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
