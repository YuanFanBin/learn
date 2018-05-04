#include "../lib/error.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXN    16384  // max # bytes request from server
#define MAXLINE 4096   // max text line length

// ../Chapter03/readn.c
ssize_t readn(int fd, void *vptr, size_t n);
// ../Chapter11/tcp_connect.c
int tcp_connect(const char *host, const char *serv);

int main(int argc, char **argv)
{
    int     i, j, fd, nchildren, nloops, nbytes;
    pid_t   pid;
    ssize_t n;
    char    request[MAXLINE], reply[MAXN];

    if (argc != 6) {
        err_quit("usage: client <hostname or IPaddr> <port> <#children> "
                 "<#loops/child> <#bytes/request>");
    }

    nchildren = atoi(argv[3]);
    nloops = atoi(argv[4]);
    nbytes = atoi(argv[5]);
    snprintf(request, sizeof(request), "%d\n", nbytes); // new line at end

    for (i = 0; i < nchildren; i++) {
        if ((pid = fork()) == 0) {  // child
            for (j = 0; j < nloops; j++) {
                fd = tcp_connect(argv[1], argv[2]);
                write(fd, request, strlen(request));
                if ((n = readn(fd, reply, nbytes)) != nbytes) {
                    err_quit("server returned %d bytes", n);
                }
                close(fd);  // TIME_WAIT on client, not server
            }
            printf("child %d done\n", i);
            exit(0);
        }
        // parent loops around to fork() again
    }
    while (wait(NULL) > 0) { } // now parent waits for all children
    if (errno != ECHILD) {
        err_sys("wait error");
    }
    exit(0);
}
