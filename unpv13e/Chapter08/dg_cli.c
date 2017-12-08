#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

#define MAXLINE 4096 /* max text line length */

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen) {
    int  n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    printf("[dg_cli.c]: sendto, recvfrom.\n");
    fflush(stdout);

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) == -1) {
            err_sys("sendto error");
        }
        if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) == -1) {
            err_sys("recvfrom error");
        }
        recvline[n] = 0; /* null terminate */
        fputs(recvline, stdout);
    }
}
