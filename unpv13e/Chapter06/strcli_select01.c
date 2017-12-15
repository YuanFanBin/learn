#include <stdio.h>      /* FILE */
#include <string.h>     /* strlen */
#include <unistd.h>     /* write */
#include <sys/select.h> /* fd_set */
#include "error.h"
#include "header.h"

#include "../Chapter03/readline.c"

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
            // 这里有stdio的缓冲区，但fgets只返回第一行数据，其他数据还在缓冲区中
            // 可能会造成select时发现fp已不可读，丢失缓冲区中的部分数据
            if (fgets(sendline, MAXLINE, fp) == NULL) {
                // 这里过早终止，会导致sockfd中的正在流动数据丢失
                return;                     /* all done, here */
            }
            if (write(sockfd, sendline, strlen(sendline)) != strlen(sendline)) {
                err_sys("write error");
            }
        }
    }
}

// 带有缓冲区的读操作（自己写或某些头文件中的读函数）与select混用时必须十分小心，
// 否则数据可能遗留在缓冲区中而导致数据丢失。根源在于select是从系统read角度看是
// 否有数据可读，并不是其他读函数角度看。
//
// 尽可能不去混用，否则处理问题的复杂度会增长到无法控制的地步。
