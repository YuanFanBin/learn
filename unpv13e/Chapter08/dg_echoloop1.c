#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define MAXLINE 4096 /* max text line length */

static void recvfrom_int(int);
static int count;

// dg_echo 统计接收到的数据报数量
void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen) {
    socklen_t   len;
    char        mesg[MAXLINE];

    signal(SIGINT, recvfrom_int);

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
