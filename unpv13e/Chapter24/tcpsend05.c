#include "../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

// ../Chapter11/tcp_connect.c
int tcp_connect(const char *host, const char *serv);

int main(int argc, char **argv)
{
    int  sockfd, size;
    char buff[16384];

    if (argc != 3) {
        err_quit("usage: tcpsend05 <host> <port#>");
    }

    if ((sockfd = tcp_connect(argv[1], argv[2])) < 0) {
        err_sys("tcp_connect error");
    }

    size = 32768;
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) < 0) {
        err_sys("setsockopt error");
    }

    write(sockfd, buff, 16384);
    printf("wrote 16384 bytes of normal data\n");
    sleep(5);

    send(sockfd, "a", 1, MSG_OOB);
    printf("wrote 1 byte of OOB data\n");

    write(sockfd, buff, 1024);
    printf("wrote 1024 bytes of normal data\n");

    exit(0);
}
