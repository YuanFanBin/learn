#include <stdio.h>  /* FILE */
#include <string.h> /* strlen */
#include <unistd.h> /* write */
#include "error.h"
#include "header.h"

#include "../Chapter05/readline.c"

#define MAXLINE 4096 /* max text line length */

#define max(a, b)   ((a) > (b) ? (a) : (b))

/*
 *  批量处理数据时，存在问题
 *  此版本仍然存在问题，当客户端从文件读入，重定向到文件时，数据产生丢失。
 *  根源在于，客户端close后，客户端进程终止，已发送数据及还未接收的数据丢失。
 */
void str_cli(FILE *fp, int sockfd)
{
    int     maxfdp1;
    fd_set  rset;
    char    sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    for ( ; ; ) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
            err_sys("select error");
        }

        if (FD_ISSET(sockfd, &rset)) {  /* socket is readable */
            if (readline(sockfd, recvline, MAXLINE) == 0) {
                err_quit("str_cli: server terminated prematurely");
            }
            if (fputs(recvline, stdout) < 0) {
                err_sys("fputs error");
            }
        }

        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
            if (fgets(sendline, MAXLINE, fp) == NULL) {
                return;                     /* all done */
            }
            if (write(sockfd, sendline, strlen(sendline)) != strlen(sendline)) {
                err_sys("write error");
            }
        }
    }
}
