#include "../lib/error.h"
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
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
    fd_set           rset;  // 2)
    sigset_t         sigset_alrm, sigset_empty; // 2)
    socklen_t        len;
    struct sockaddr *preply_addr;

    preply_addr = (struct sockaddr *) malloc(servlen);

    // 设置 SO_BROADCAST
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
        err_sys("setsockopt fail");
    }

    // 2
    FD_ZERO(&rset);
    sigemptyset(&sigset_empty);
    sigemptyset(&sigset_alrm);
    sigaddset(&sigset_alrm, SIGALRM);

    if (signal(SIGALRM, recvfrom_alarm) < 0) {
        err_sys("signal fail");
    }

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);
        alarm(1);
        for ( ; ; ) {
            FD_SET(sockfd, &rset);
            n = pselect(sockfd + 1, &rset, NULL, NULL, NULL, &sigset_empty);
            if (n < 0) {
                if (errno == EINTR) {
                    break;
                }
                err_sys("pselect error");
            } else if (n != 1) {
                err_sys("pselect error: returned %d", n);
            }
            len = servlen;
            n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
            recvline[n] = 0;
            printf("from %s: %s", sock_ntop_host(preply_addr, len), recvline);
        }
    }
    free(preply_addr);
}

static void recvfrom_alarm(int signo)
{
    return;     // just interrupt the recvfrom()
}
