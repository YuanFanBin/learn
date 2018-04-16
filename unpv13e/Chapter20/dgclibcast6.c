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
#define max(a, b)   ((a) > (b) ? (a) : (b))

static int pipefd[2];
static void recvfrom_alarm(int signo);
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int              n, maxfdp1;
    const int        on = 1;
    char             sendline[MAXLINE], recvline[MAXLINE + 1];
    fd_set           rset;
    socklen_t        len;
    struct sockaddr *preply_addr;

    preply_addr = (struct sockaddr *) malloc(servlen);

    // 设置 SO_BROADCAST
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
        err_sys("setsockopt fail");
    }

    if (pipe(pipefd) < 0) {
        err_sys("pipe failed");
    }
    maxfdp1 = max(sockfd, pipefd[0]) + 1;
    FD_ZERO(&rset);

    if (signal(SIGALRM, recvfrom_alarm) < 0) {
        err_sys("signal fail");
    }

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        alarm(1);
        for ( ; ; ) {
            FD_SET(sockfd, &rset);
            FD_SET(pipefd[0], &rset);
            if ((n = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
                if (errno == EINTR) {
                    continue;
                }
                err_sys("select error");
            }
            if (FD_ISSET(sockfd, &rset)) {
                len = servlen;
                n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
                recvline[n] = 0;
                printf("from %s: %s", sock_ntop_host(preply_addr, len), recvline);
            }
            if (FD_ISSET(pipefd[0], &rset)) {
                read(pipefd[0], &n, 1); // timer expired
                break;
            }
        }
    }
    free(preply_addr);
}

static void recvfrom_alarm(int signo)
{
    write(pipefd[1], "", 1);    // write one null byte to pipe
}
