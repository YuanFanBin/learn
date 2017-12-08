#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "error.h"

#define SERV_PORT 9877 /* TCP and UDP client-servers */

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen);

int main(int argc, char **argv) {
    int                sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        err_quit("usage: udpcli <IPaddress>");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT); // #1, #3
    // $ grep "echo" /etc/service
    // echo                7/tcp
    // echo                7/udp
    // servaddr.sin_port = htons(7);         // #2
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) == -1) {
        err_sys("inet_pton error");
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        err_sys("socket error");
    }
    dg_cli(stdin, sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    exit(0);
}

// #1
// $ gcc udpcli01.c error.c error.h dg_cli.c -o udpcli
// #2
// $ gcc udpcli01.c error.c error.h dg_cliaddr.c sock_ntop.c -o udpcli
// #3
// $ gcc udpcli01.c error.c error.h dg_cliconnect.c -o udpcli
// #4
// $ gcc udpcli01.c error.c error.h dg_cliloop1.c -o udpcli
