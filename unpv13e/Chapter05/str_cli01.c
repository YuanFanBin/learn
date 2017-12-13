#include <stdio.h>  /* FILE */
#include <string.h> /* strlen */
#include <unistd.h> /* write */
#include "../lib/error.h"

#define MAXLINE 4096    /* max text line length */

ssize_t readline(int fd, void *vptr, size_t maxlen);

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        /* 引发RST */
        if (write(sockfd, sendline, 1) != 1) {
            err_sys("write error");
        }
        sleep(1);
        /* 产生SIGPIPE */
        if (write(sockfd, sendline + 1, strlen(sendline) - 1)
                != strlen(sendline) - 1)
        {
            err_sys("write error");
        }
        if (readline(sockfd, recvline, MAXLINE) == 0) {
            err_quit("str_cli: server terminated prematurely");
        }
        if (fputs(recvline, stdout) == EOF) {
            err_sys("fputs error");
        }
    }
}
