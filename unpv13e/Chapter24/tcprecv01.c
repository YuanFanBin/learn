#include "../lib/error.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int listenfd, connfd;

// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);
void sig_urg(int signo);

int main(int argc, char **argv)
{
    int     n;
    char    buff[100];

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], NULL);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], NULL);
    } else {
        err_quit("usage: tcprecv01 [ <host> ] <port#>");
    }

    if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
        err_sys("accept error");
    }
    signal(SIGURG, sig_urg);
    fcntl(connfd, F_SETOWN, getpid());

    for ( ; ; ) {
        if ((n = read(connfd, buff, sizeof(buff) - 1)) == 0) {
            printf("received EOF\n");
            exit(0);
        }
        buff[n] = 0;
        printf("read %d bytes: %s\n", n, buff);
    }
}

void sig_urg(int signo)
{
    int     n;
    char    buff[100];

    printf("SIGURG received(%d)\n", signo);
    if ((n = recv(connfd, buff, sizeof(buff) - 1, MSG_OOB)) < 0) {
        err_sys("recv error");
    }
    buff[n] = 0;
    printf("read %d OOB byte: %s\n", n, buff);
}
