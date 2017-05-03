#include <unistd.h>     /* ssize_t, read, write */
#include <errno.h>      /* errno */
#include "error.h"

#define MAXLINE 4096    /* max text line length */

void str_echo(int sockfd)
{
    ssize_t n;
    char    buf[MAXLINE];
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        if (write(sockfd, buf, n) != n) {
            err_sys("write error");
        }
    }
    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        err_sys("str_echo: read error");
    }
}
