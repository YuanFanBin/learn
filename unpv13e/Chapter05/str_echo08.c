#include "../lib/error.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096    /* max text line length */

// ../Chapter03/readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);

/* 客户端与服务端之间传递文本字符串 */
void str_echo(int sockfd)
{
    long    arg1, arg2;
    ssize_t n;
    char    line[MAXLINE];

    for ( ; ; ) {
        if ((n = readline(sockfd, line, MAXLINE)) == 0) {
            return;     /* connection closed by other end */
        }
        if (sscanf(line, "%ld%ld", &arg1, &arg2) == 2) {
            snprintf(line, sizeof(line), "%ld\n", arg1 + arg2);
        } else {
            snprintf(line, sizeof(line), "input error\n");
        }
        n = strlen(line);
        if (write(sockfd, line, n) != n) {
            err_sys("write error");
        }
    }
}
