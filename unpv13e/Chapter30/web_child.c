#include "../lib/error.h"
#include <stdlib.h>
#include <sys/types.h>

#define MAXN    16384  // max # bytes request from server
#define MAXLINE 4096   // max text line length

// ../Chapter03/writen.c
ssize_t writen(int fd, const void *vptr, size_t n);
// ../Chapter26/readline.c, readline 线程安全版本
ssize_t readline(int fd, void *vptr, size_t maxlen);

void web_child(int sockfd)
{
    int     ntowrite;
    ssize_t nread;
    char    line[MAXLINE], result[MAXN];

    for ( ; ; ) {
        if ((nread = readline(sockfd, line, MAXLINE)) == 0) {
            return; // connection closed by other end
        }
        // line from client specifies #bytes to write back
        ntowrite = atol(line);
        if ((ntowrite <= 0) || (ntowrite > MAXN)) {
            err_quit("client request for %d bytes", ntowrite);
        }
        writen(sockfd, result, ntowrite);
    }
}
