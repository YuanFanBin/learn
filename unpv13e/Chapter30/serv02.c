#include "../lib/error.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int nchildren;
static pid_t *pids;

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int main(int argc, char **argv)
{
    int       listenfd, i;
    socklen_t addrlen;
    void      sig_int(int);
    pid_t     child_make(int, int, int);

    if (argc == 3) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 4) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: serv02 [ <host> ] <port#> <#children>");
    }
    nchildren = atoi(argv[argc-1]);
    pids = (pid_t *) calloc(nchildren, sizeof(pid_t));

    for (i = 0; i < nchildren; i++) {
        pids[i] = child_make(i, listenfd, addrlen); // parent returns
    }
    signal(SIGINT, sig_int);

    for ( ; ; ) {
        pause();    // everything done by children
    }
}

void sig_int(int signo)
{
    int  i;
    void pr_cpu_time(void);

    // terminate all children
    for (i = 0; i < nchildren; i++) {
        kill(pids[1], SIGTERM);
    }
    while (wait(NULL) > 0) { } // wait for all children

    if (errno != ECHILD) {
        err_sys("wait error");
    }
    pr_cpu_time();
    exit(0);
}
