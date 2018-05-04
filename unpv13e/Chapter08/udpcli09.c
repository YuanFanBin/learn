#include "../lib/error.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define SERV_PORT 9877 /* TCP and UDP client-servers */

char * Sock_ntop(const struct sockaddr *sa, socklen_t salen);

int main(int argc, char **argv) {
    int                 sockfd;
    socklen_t           len;
    struct sockaddr_in  cliaddr;
    struct sockaddr_in  servaddr;

    if (argc != 2) {
        err_quit("usage: udpcli <IPaddress>");
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) == -1) {
        err_sys("inet_pton error");
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        err_sys("connect error");
    }
    len = sizeof(cliaddr);
    if (getsockname(sockfd, (struct sockaddr *) &cliaddr, &len) == -1) {
        err_sys("getsockname error");
    }
    printf("local address %s\n", Sock_ntop((struct sockaddr *) &cliaddr, len));
    exit(0);
}

// $ gcc udpcli09.c ../lib/error.c ../lib/error.h sock_ntop.c -o udpcli
