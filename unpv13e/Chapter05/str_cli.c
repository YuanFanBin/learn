#include <stdio.h>      /* FILE */
#include <string.h>     /* strlen */
#include <unistd.h>     /* write */
#include "../lib/error.h"

#define MAXLINE     4096    /* max text line length */

// Chapter03/readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (write(sockfd, sendline, strlen(sendline)) != strlen(sendline)) {
            err_sys("write error");
        }
        if (readline(sockfd, recvline, MAXLINE) == 0) {
            err_quit("str_cli: serve terminate permaturely");
        }
        if (fputs(recvline, stdout) == EOF) {
            err_sys("fputs error");
        }
    }
}

