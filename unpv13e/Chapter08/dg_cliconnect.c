#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../lib/error.h"

#define MAXLINE 4096 /* max text line length */

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen) {
    int  n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    printf("[dg_cliconnect.c]: connect, write, read.\n");
    fflush(stdout);

    if (connect(sockfd, (struct sockaddr *) pservaddr, servlen) == -1) {
        err_sys("connect error");
    }

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (write(sockfd, sendline, strlen(sendline)) == -1) {
            err_sys("write error");
        }
        if ((n = read(sockfd, recvline, MAXLINE)) == -1) {
            err_sys("recvfrom error");
        }
        recvline[n] = 0; /* null terminate */
        fputs(recvline, stdout);
    }
}
