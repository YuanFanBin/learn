#include "../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

// ../Chapter11/tcp_connect.c
int tcp_connect(const char *host, const char *serv);

int main(int argc, char **argv)
{
    int sockfd;

    if (argc != 3) {
        err_quit("usage: tcpsend04 <host> <port#>");
    }

    if ((sockfd = tcp_connect(argv[1], argv[2])) < 0) {
        err_sys("tcp_connect error");
    }

    write(sockfd, "123", 3);
    printf("wrote 3 bytes of normal data\n");

    send(sockfd, "4", 1, MSG_OOB);
    printf("wrote 1 byte of OOB data\n");

    write(sockfd, "5", 1);
    printf("wrote 1 byte of normal data\n");

    exit(0);
}
