#include "../lib/error.h"
#include "child.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define max(a, b)   ((a) > (b) ? (a) : (b))

static int nchildren;

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);
// ../../Chapter15/write_fd.c
ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd);

int main(int argc, char **argv)
{
    int              listenfd, i, navail, maxfd, nsel, connfd, rc;
    void             sig_int(int);
    pid_t            child_make(int, int, int);
    ssize_t          n;
    fd_set           rset, masterset;
    socklen_t        addrlen, clilen;
    struct sockaddr *cliaddr;

    if (argc == 3) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 4) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: serv05 [ <host> ] <port#> <#children>");
    }

    FD_ZERO(&masterset);
    FD_SET(listenfd, &masterset);
    maxfd = listenfd;
    cliaddr = (struct sockaddr *) malloc(addrlen);

    nchildren = atoi(argv[argc-1]);
    navail = nchildren;
    cptr = (Child *) calloc(nchildren, sizeof(Child));

    // prefork all the children
    for (i = 0; i < nchildren; i++) {
        child_make(i, listenfd, addrlen);   // parent returns
        FD_SET(cptr[i].child_pipefd, &masterset);
        maxfd = max(maxfd, cptr[i].child_pipefd);
    }

    signal(SIGINT, sig_int);

    for ( ; ; ) {
        rset = masterset;
        if (navail <= 0) {
            FD_CLR(listenfd, &rset); // turn off if no available children
        }
        nsel = select(maxfd + 1, &rset, NULL, NULL, NULL);

        // check for new connections
        if (FD_ISSET(listenfd, &rset)) {
            clilen = addrlen;
            if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
                err_sys("accept error");
            }

            for (i = 0; i < nchildren; i++) {
                if (cptr[i].child_status == 0) {
                    break;  // avaiable
                }
            }
            if (i == nchildren) {
                err_quit("no available children");
            }
            cptr[i].child_status = 1;   // mark child as busy
            cptr[i].child_count++;
            navail--;

            n = write_fd(cptr[i].child_pipefd, "", 1, connfd);
            close(connfd);
            if (--nsel == 0) {
                continue;   // all done with select() results
            }
        }

        // find any newly-available children
        for (i = 0; i < nchildren; i++) {
            if (FD_ISSET(cptr[i].child_pipefd, &rset)) {
                // 有子进程意外终止，父进程也随之终止
                // 更好的做法是重新派生一个子进程取代意外终止的那个进程
                if ((n = read(cptr[i].child_pipefd, &rc, 1)) == 0) {
                    err_quit("child %d terminated unexpectedly", i);
                }
                cptr[i].child_status = 0;
                navail++;
                if (--nsel == 0) {
                    break;  // all done with select() results
                }
            }
        }
    }
}

void sig_int(int signo)
{
    int  i;
    void pr_cpu_time(void);

    // terminate all children
    for (i = 0; i < nchildren; i++) {
        kill(cptr[i].child_pid, SIGTERM);
    }
    while (wait(NULL) > 0) { } // wait for all children

    if (errno != ECHILD) {
        err_sys("wait error");
    }
    pr_cpu_time();

    for (i = 0; i < nchildren; i++) {
        printf("child %d, %ld connections\n", i, cptr[i].child_count);
    }

    exit(0);
}
