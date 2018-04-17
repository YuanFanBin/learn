#include "../lib/error.h"
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */

static void recvfrom_alarm(int signo);
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int              n;
    const int        on = 1;
    char             sendline[MAXLINE], recvline[MAXLINE + 1];
    sigset_t         sigset_alrm;
    socklen_t        len;
    struct sockaddr *preply_addr;

    preply_addr = (struct sockaddr *) malloc(servlen);

    // 设置 SO_BROADCAST
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
        err_sys("setsockopt fail");
    }

    sigemptyset(&sigset_alrm);
    if (signal(SIGALRM, recvfrom_alarm) < 0) {
        err_sys("signal fail");
    }

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        alarm(1);
        for ( ; ; ) {
            len = servlen;
            sigprocmask(SIG_UNBLOCK, &sigset_alrm, NULL);
            n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
            sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);
            if (n < 0) {
                if (errno == EINTR) {
                    break;      // wait long ehough for replies
                }
                err_sys("redcvfrom error");
            } else {
                recvline[n] = 0;
                printf("from %s: %s", sock_ntop_host(preply_addr, len), recvline);
            }
        }
    }
    free(preply_addr);
}

static void recvfrom_alarm(int signo)
{
    return;     // just interrupt the recvfrom()
}