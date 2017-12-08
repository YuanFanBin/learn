#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define MAXLINE 4096 /* max text line length */

static void recvfrom_int(int);
static int count;

// dg_echo 统计接收到的数据报数量
void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen) {
    int         n;
    socklen_t   len;
    char        mesg[MAXLINE];

    signal(SIGINT, recvfrom_int);

    n = 220 * 1024;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n)) == -1) {
        err_sys("setsockopt error");
    }

    for (;;) {
        len = clilen;
        if (recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &clilen) == -1) {
            err_sys("recvfrom error");
        }
        count++;
    }
}

static void recvfrom_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}
