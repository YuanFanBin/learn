#include "../lib/error.h"
#include <errno.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */

static sigjmp_buf jmpbuf;
static void recvfrom_alarm(int signo);
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int              n;
    const int        on = 1;
    char             sendline[MAXLINE], recvline[MAXLINE + 1];
    socklen_t        len;
    struct sockaddr *preply_addr;

    preply_addr = (struct sockaddr *) malloc(servlen);

    // 设置 SO_BROADCAST
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
        err_sys("setsockopt fail");
    }

    if (signal(SIGALRM, recvfrom_alarm) < 0) {
        err_sys("signal fail");
    }

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        alarm(1);
        for ( ; ; ) {
            if (sigsetjmp(jmpbuf, 1) != 0) {
                break;
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
    siglongjmp(jmpbuf, 1);
}
