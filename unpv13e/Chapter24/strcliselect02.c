#include "../lib/error.h"
#include <errno.h>
#include <stdio.h>          /* FILE */
#include <string.h>         /* strlen */
#include <sys/select.h>
#include <sys/socket.h>     /* shutdown */
#include <unistd.h>         /* write */

#define MAXLINE 4096    /* max text line length */

#define max(a, b) ((a) > (b) ? (a) : (b))

// ../Chapter03/writen.c
ssize_t writen(int fd, const void *vptr, size_t n);
// ../Chapter03/readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);
// ./heartbeat_cli.c
void heartbeat_cli(int servfd_arg, int nsec_arg, int maxnprobes_arg);

// ../Chapter05/strcli_select02.c
void str_cli(FILE *fp, int sockfd)
{
    int     maxfdp1, stdineof = 0;
    fd_set  rset;
    char    sendline[MAXLINE], recvline[MAXLINE];

    heartbeat_cli(sockfd, 1, 5);

    FD_ZERO(&rset);
    for ( ; ; ) {
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
            if (errno == EINTR) {
                continue;
            }
            err_sys("select error");
        }

        if (FD_ISSET(sockfd, &rset)) {  /* socket is readable */
            if (readline(sockfd, recvline, MAXLINE) == 0) {
                if (stdineof == 1) {    // sockfd 已无数据，并且fp数据已读完
                    return;
                }
                // sockfd 已无数据，但fp数据未读完，数据未传输完
                err_quit("str_cli: server terminated prematurely");
            }
            writen(STDOUT_FILENO, recvline, strlen(recvline));
        }
        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
            if (fgets(sendline, MAXLINE, fp) == NULL) {
                // 从文件中读完数据，关闭对sockfd的写端
                stdineof = 1;
                alarm(0);
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset); // 并清除select对fp的等待
                continue;
            }
            writen(sockfd, sendline, strlen(sendline));
        }
    }
}
