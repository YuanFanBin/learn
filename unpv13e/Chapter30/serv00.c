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

// 迭代服务器程序
int main(int argc, char **argv)
{
    int              listenfd, connfd;
    void             sig_int(int), web_child(int);
    socklen_t        clilen, addrlen;
    struct sockaddr *cliaddr;

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: serv00 [ <host> ] <port#>");
    }
    cliaddr = (struct sockaddr *) malloc(addrlen);

    signal(SIGINT, sig_int);

    for ( ; ; ) {
        clilen = addrlen;
        if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
            err_sys("accept error");
        }
        web_child(connfd);  // process the request
        close(connfd);      // parent closes connected socket
    }
}

void sig_int(int signo)
{
    void pr_cpu_time(void);

    pr_cpu_time();
    exit(0);
}
