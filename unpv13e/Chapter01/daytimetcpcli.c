/* intro/daytimetcpcli */
#include <arpa/inet.h>  /* inet_pton */
#include <errno.h>      /* errno */
#include <netinet/in.h> /* socket, sockaddr_in */
#include <stdio.h>      /* fputs */
#include <stdlib.h>
#include <strings.h>    /* bzero */
#include <sys/socket.h> /* connect */
#include <sys/types.h>  /* connect, read */
#include <sys/uio.h>    /* read */
#include <unistd.h>     /* read */
#include "error.h"      /* err_* */

#define MAXLINE 4096    /* max text line length */

/* figure-1.5 */
/* $ gcc error.h daytimetcpcli.c */
int main(int argc, char **argv)
{
    int                 sockfd, n;
    char                recvline[MAXLINE + 1];
    struct sockaddr_in  servaddr;

    if (argc != 2) {
        err_quit("usage: a.out <IPaddress>");
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    /* /etc/services 中查看端口对应的服务 */
    servaddr.sin_port = htons(13);      /* daytime server */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        err_quit("inet_pton error for %s", argv[1]);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        err_sys("connect error, errno = %d", errno);
    }

    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;        /* null terminate */
        if (fputs(recvline, stdout) == EOF) {
            err_sys("fputs error");
        }
    }
    if (n < 0) {
        err_sys("read error");
    }
    exit(0);
}
