#include "../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int main(int argc, char **argv)
{
    int  listenfd, connfd, n, on = 1;
    char buff[100];

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], NULL);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], NULL);
    } else {
        err_quit("usage: tcprecv04 [ <host> ] <port#>");
    }

    if (setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(on)) < 0) {
        err_sys("setsockopt error");
    }

    if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
        err_sys("accept error");
    }
    sleep(5);

    for ( ; ; ) {
        if (sockatmark(connfd) < 0) {
            printf("at OOB mark\n");
        }

        if ((n = read(connfd, buff, sizeof(buff) - 1)) == 0) {
            printf("received EOF\n");
            exit(0);
        }
        buff[n] = 0;
        printf("read %d bytes: %s\n", n, buff);
    }
}
