#include "../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

// 并发服务器程序，每个客户一个子进程
int main(int argc, char **argv)
{
    int              listenfd, connfd;
    pid_t            childpid;
    void             sig_chld(int), sig_int(int), web_child(int);
    socklen_t        clilen, addrlen;
    struct sockaddr *cliaddr;

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: serv01 [ <host> ] <port#>");
    }
    cliaddr = (struct sockaddr *) malloc(addrlen);

    signal(SIGCHLD, sig_chld);
    signal(SIGINT, sig_int);

    for ( ; ; ) {
        clilen = addrlen;
        if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
            if (errno == EINTR) {
                continue;
            }
            err_sys("accept error");
        }
        if ((childpid = fork()) == 0) { // child
            close(listenfd);    // close listening socket
            web_child(connfd);  // process the request
            exit(0);
        }
        close(connfd);      // parent closes connected socket
    }
}

void sig_int(int signo)
{
    void pr_cpu_time(void);

    pr_cpu_time();
    exit(0);
}
