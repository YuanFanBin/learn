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
        err_quit("usage: tcpsend01 <host> <port#>");
    }
    if ((sockfd = tcp_connect(argv[1], argv[2])) < 0) {
        err_sys("tcp_connect error");
    }

    write(sockfd, "123", 3);
    printf("wrote 3 bytes of normal data\n");
    sleep(1); // 让每个write或send的数据作为单个TCP分片在本端发送并在对端接收

    send(sockfd, "4", 1, MSG_OOB);
    printf ("wrote 1 byte of OOB data\n");
    sleep(1);

    write(sockfd, "56", 2);
    printf("wrote 2 bytes of normal data\n");
    sleep(1);

    send(sockfd, "7", 1, MSG_OOB);
    printf("wrote 1 byte of OOB data\n");
    sleep(1);

    write(sockfd, "89", 2);
    printf("wrote 2 bytes of normal data\n");
    sleep(1);

    exit(0);
}
