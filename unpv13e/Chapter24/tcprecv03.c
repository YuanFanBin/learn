#include "../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

// （不正确地）使用select得到带外数据通知的接收程序
int main(int argc, char **argv)
{
    int     listenfd, connfd, n, justreadoob = 0;
    char    buff[100];
    fd_set  rset, xset;

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], NULL);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], NULL);
    } else {
        err_quit("usage: tcprecv02 [ <host> ] <port#>");
    }

    if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
        err_sys("accept error");
    }

    FD_ZERO(&rset);
    FD_ZERO(&xset);
    for ( ; ; ) {
        FD_SET(connfd, &rset);
        if (justreadoob == 0) {
            FD_SET(connfd, &xset);
        }

        if (select(connfd + 1, &rset, NULL, &xset, NULL) < 0) {
            err_sys("select error");
        }

        if (FD_ISSET(connfd, &xset)) {
            n = recv(connfd, buff, sizeof(buff) - 1, MSG_OOB);
            buff[n] = 0;
            printf("read %d OOB byte: %s\n", n, buff);
            justreadoob = 1;
            FD_CLR(connfd, &xset);
        }

        if (FD_ISSET(connfd, &rset)) {
            if ((n = read(connfd, buff, sizeof(buff) - 1)) == 0) {
                printf("received EOF\n");
                exit(0);
            }
            buff[n] = 0;
            printf("read %d bytes: %s\n", n, buff);
            justreadoob = 0;
        }
    }
}
