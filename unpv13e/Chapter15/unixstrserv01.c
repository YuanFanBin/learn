#include "../lib/error.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIXSTR_PATH    "/tmp/unix.str"     /* Unix domain stream */
#define LISTENQ 1024            /* 2nd argument to listen() */

// Chapter05/str_echo.c
void str_echo(int sockfd);
// Chapter/05/sigchildwaitpid.c
void sig_chld(int);

int main(int argc, char **argv)
{
    int                 listenfd, connfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_un  cliaddr, servaddr;

    if ((listenfd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1)  {
        err_sys("socket error");
    }

    unlink(UNIXSTR_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        err_sys("bind error");
    }
    if (listen(listenfd, LISTENQ) == -1) {
        err_sys("listen error");
    }

    signal(SIGCHLD, sig_chld);

    for (;;) {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
            if (errno == EINTR) {
                continue;
            }
            err_sys("accept error");
        }
        if ((childpid = fork()) == -1) {
            err_sys("fork error");
        } else if (childpid == 0) { // child
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
}

// $ gcc unixstrserv01.c ../lib/error.c ../Chapter05/str_echo.c ../Chapter05/sigchildwaitpid.c -o unixstrserv01
