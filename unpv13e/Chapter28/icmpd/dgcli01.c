#include "../../lib/error.h"
#include "unpicmpd.h"
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define MAXLINE     4096        /* max text line length */
#define max(a, b)   ((a) > (b) ? (a) : (b))

// sock_bind_wild.c
int sock_bind_wild(int sockfd, int family);
// ../../lib/sock_ntop.c
char *sock_ntop(const struct sockaddr *sa, socklen_t salen);
// ../../Chapter15/write_fd.c
ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd);

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int                 icmpfd, maxfdp1;
    char                sendline[MAXLINE], recvline[MAXLINE + 1];
    fd_set              rset;
    ssize_t             n;
    struct timeval      tv;
    struct icmpd_err    icmpd_err;
    struct sockaddr_un  sun;

    // 绑定通配地址和临时端口
    sock_bind_wild(sockfd, pservaddr->sa_family);

    if ((icmpfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }
    sun.sun_family = AF_LOCAL;
    strcpy(sun.sun_path, ICMPD_PATH);
    if (connect(icmpfd, (struct sockaddr *) &sun, sizeof(sun)) < 0) {
        err_sys("connect error");
    }
    write_fd(icmpfd, "1", 1, sockfd);
    n = read(icmpfd, recvline, 1);
    if (n != 1 || recvline[0] != '1') {
        err_quit("error creating icmp socket, n = %d, char = %c", n, recvline[0]);
    }

    FD_ZERO(&rset);
    maxfdp1 = max(sockfd, icmpfd) + 1;
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        tv.tv_sec = 5;
        tv.tv_usec = 0;
        FD_SET(sockfd, &rset);
        FD_SET(icmpfd, &rset);
        if ((n = select(maxfdp1, &rset, NULL, NULL, &tv)) == 0) {
            fprintf(stderr, "socket timeout\n");
            continue;
        }

        if (FD_ISSET(sockfd, &rset)) {
            n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
            recvline[n] = 0;
            fputs(recvline, stdout);
        }

        // 处理ICMP错误
        if (FD_ISSET(icmpfd, &rset)) {
            if ((n = read(icmpfd, &icmpd_err, sizeof(icmpd_err))) == 0) {
                err_quit("ICMP daemon terminated");
            } else if (n != sizeof(icmpd_err)) {
                err_quit("n = %d, expected %d", n, sizeof(icmpd_err));
            }
            printf("ICMP error: dest = %s, %s, type = %d, code = %d\n", sock_ntop((struct sockaddr *) &icmpd_err.icmpd_dest, icmpd_err.icmpd_len), strerror(icmpd_err.icmpd_errno), icmpd_err.icmpd_type, icmpd_err.icmpd_code);
        }
    }
}
