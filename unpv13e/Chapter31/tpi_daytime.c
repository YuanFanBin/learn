#include "../lib/error.h"
#include "tpi_daytime.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define MAXLINE 4096

int main(int argc, char **argv)
{
    int                 fd, n;
    char                recvline[MAXLINE + 1];
    struct sockaddr_in  myaddr, servaddr;

    if (argc != 2) {
        err_quit("usage: tpi_daytime <IPaddress>");
    }

    if ((fd = open(XTI_TCP, ORDWR, 0)) < 0) {
        err_sys("open error");
    }

    // bind any local address
    bzero(&myaddr, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(0);

    tpi_bind(fd, &myaddr, sizeof(struct sockaddr_in));

    // fill in server's address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);  // daytime server
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    tpi_connect(fd, &servaddr, sizeof(struct sockaddr_in));

    for ( ; ; ) {
        if ((n = tip_read(fd, recvline, MAXLINE)) <= 0) {
            if (n == 0) {
                break;
            }
            err_sys("tpi_read error");
        }
        recvline[n] = 0;
        fputs(recvline, stdout);
    }
    tpi_close(fd);
    exit(0);
}
