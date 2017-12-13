#include <sys/socket.h>     /* socket */
#include <netinet/in.h>     /* sockaddr_in */
#include <arpa/inet.h>      /* inet_pton */
#include <strings.h>         /* bzero */
#include <errno.h>          /* errno */
#include <stdlib.h>         /* exit */
#include <stdio.h>
#include "../lib/error.h"

#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define MAXLINE     4096    /* max text line length */

void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
    int                 n;
    int                 sockfd;
    struct sockaddr_in  servaddr;

    if (argc != 2) {
        err_quit("usage: tcpcli <IPaddress>");
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if ((n = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) < 0) {
        err_sys("inet_pton error for %s", argv[1]); /* errno set */
    } else if (n == 0) {
        err_quit("inet_pton error for %s", argv[1]);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        err_sys("connect error, errno = %d", errno);
    }

    str_cli(stdin, sockfd); /* do it all */
    exit(0);
}
