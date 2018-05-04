#include "../lib/error.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define MAXLINE 4096 /* max text line length */

int readable_timeo(int fd, int sec);

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) == -1) {
            err_sys("sendto error");
        }
        if (readable_timeo(sockfd, 5) == 0) {
            fprintf(stderr, "socket timeout\n");
        } else {
            if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) == -1) {
                err_sys("recvfrom error");
            }
            recvline[n] = '\0';
            fputs(recvline, stdout);
        }
    }
}
