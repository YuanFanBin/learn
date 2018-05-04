#include "../lib/error.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int main(int argc, char **argv)
{
    int              listenfd, connfd, e;
    void             sig_int(int);
    void            *doit(void *);
    pthread_t        tid;
    socklen_t        clilen, addrlen;
    struct sockaddr *cliaddr;

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: serv06 [ <host> ] <port#>");
    }
    cliaddr = (struct sockaddr *) malloc(addrlen);

    signal(SIGINT, sig_int);

    for ( ; ; ) {
        clilen = addrlen;
        if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
            err_sys("accept error");
        }
        if ((e = pthread_create(&tid, NULL, &doit, (void *) connfd)) != 0) {
            errno = e;
            err_sys("pthread_create error");
        }
    }
}

void *doit(void *arg)
{
    int e;
    void web_child(int);

    if ((e = pthread_detach(pthread_self())) != 0) {
        errno = e;
        err_sys("pthread_detach error");
    }
    web_child((int) arg);
    close((int) arg);
    return(NULL);
}

void sig_int(int signo)
{
    void pr_cpu_time(void);

    pr_cpu_time();
    exit(0);
}
