#include "../lib/error.h"
#include <netinet/in.h>

#define MAXLINE 4096 /* max text line length */

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen) {
    int         n;
    socklen_t   len;
    char        mesg[MAXLINE];

    for (;;) {
        len = clilen;
        if ((n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &clilen)) == -1) {
            err_sys("recvfrom error");
        }
        if (sendto(sockfd, mesg, n, 0, pcliaddr, len) == -1) {
            err_sys("sendto error");
        }
    }
}
