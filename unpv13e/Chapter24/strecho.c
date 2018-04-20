#include "../lib/error.h"
#include <errno.h>      /* errno */
#include <unistd.h>     /* ssize_t, read, write */

#define MAXLINE 4096    /* max text line length */

// ../Chapter03/writen.c
ssize_t writen(int fd, const void *vptr, size_t n);
// ../Chapter03/readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);
// ./heartbeat_serv.c
void heartbeat_serv(int servfd_arg, int nsec_arg, int maxnprobes_arg);

void str_echo(int sockfd)
{
    ssize_t n;
    char    line[MAXLINE];

    heartbeat_serv(sockfd, 1, 5);

    for ( ; ; ) {
        if ((n = readline(sockfd, line, MAXLINE)) == 0) {
            return;
        }
        writen(sockfd, line, n);
    }
}
