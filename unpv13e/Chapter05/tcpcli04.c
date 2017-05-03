#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* sockaddr_in */
#include <arpa/inet.h>  /* inet_pton */
#include <string.h>     /* bzero */
#include <errno.h>      /* errno */
#include "error.h"

#include "str_cli.c"

#define SERV_PORT   9877    /* TCP and UDP client-servers */

int main(int argc, char **argv)
{
    int                 i, n, sockfd[5];
    struct sockaddr_in  servaddr;

    if (argc != 2) {
        err_quit("usage: tcpcli <IPaddress>");
    }

    for (i = 0; i < 5; i++) {
        if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
        if (connect(sockfd[i], (struct sockaddr *) &servaddr,
                    sizeof(servaddr)) < 0)
        {
            err_sys("connect error, errno = %d", errno);
        }
    }
    str_cli(stdin, sockfd[0]); /* do it all */
    exit(0);
}
