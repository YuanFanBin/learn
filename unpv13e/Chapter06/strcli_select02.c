#include "../lib/error.h"
#include "header.h"
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../Chapter03/readline.c"

#define MAXLINE 4096    /* max text line length */

#define max(a, b) ((a) > (b) ? (a) : (b))

void str_cli(FILE *fp, int sockfd)
{
    int     maxfdp1, stdineof;
    fd_set  rset;
    char    buf[MAXLINE];
    int     n;

    stdineof = 0;
    FD_ZERO(&rset);
    for ( ; ; ) {
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
            err_sys("select error");
        }

        if (FD_ISSET(sockfd, &rset)) {  /* socket is readable */
            if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1) {
                    // sockfd 已无数据，并且fp数据已读完
                    return;             /* normal termination */
                }
                // sockfd 已无数据，但fp数据未读完，数据未传输完
                err_quit("str_cli: server terminated prematurely");
            }
            if (write(fileno(stdout), buf, n) != n) {
                err_sys("write error");
            }
        }
        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
            if ((n = read(fileno(fp), buf, MAXLINE)) == 0) {
                // 从文件中读完数据，关闭对sockfd的写端
                stdineof = 1;
                if (shutdown(sockfd, SHUT_WR) < 0) { /* send FIN */
                    err_sys("shutdown error");
                }
                // 并清除select对fp的等待
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            if (write(sockfd, buf, n) != n) {
                err_sys("write error");
            }
        }
    }
}
