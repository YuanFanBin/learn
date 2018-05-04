#include "../lib/error.h"
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */

static void sig_alrm(int signo);

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int  n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    signal(SIGALRM, sig_alrm);

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) == -1) {
            err_sys("sendto error");
        }
        alarm(5);
        if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) == -1) {
            if (errno == EINTR) {
                fprintf(stderr, "socket timeout\n");
            } else {
                err_sys("recvfrom error");
            }
        } else {
            alarm(0);
            recvline[0] = '\0';
            fputs(recvline, stdout);
        }
    }
}

static void sig_alrm(int signo)
{
    // just interrupt the recvfrom()
}
