#include <arpa/inet.h>      /* htonl, htons */
#include <netinet/in.h>     /* sockaddr_in */
#include <strings.h>        /* bzero */
#include <sys/socket.h>     /* socklen_t */
#include <stdlib.h>         /* exit */
#include <unistd.h>         /* fork, read, write */
#include <errno.h>          /* errno */
#include "error.h"

#include "str_echo.c"
#include "signal.c"
#include "sigchildwait.c"   /* 使用wait可能产生僵死进程 */
/*
 *  多个连接同时断开时，信号处理机制没有排队功能，仅有一个子进程被wait，
 *  其他进程进入僵死状态
 */

#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define LISTENQ     1024    /* 2nd argument to listen() */
#define MAXLINE     4096    /* max text line length */

/* gcc tcpserv01.c */
int main(int argc, char **argv)
{
    int                 err;
    int                 listenfd, connfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_in  cliaddr, servaddr;

    signal(SIGCHLD, sig_child);     /* ADD */

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ((err = bind(listenfd, (struct sockaddr *) &servaddr,
                    sizeof(servaddr))) < 0)
    {
        err_sys("bind error");
    }

    if ((err = listen(listenfd, LISTENQ)) < 0) {
        err_sys("listen error");
    }

    for ( ; ; ) {
again:
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr,
                             &clilen)) < 0)
        {
#ifdef EPROTP
            if (errno == EPROTO || errno == ECONNABORTED) {
#else
            if (errno == ECONNABORTED) {
#endif
                goto again;
            } else {
                err_sys("accept error");
            }
        }
        if ((childpid = fork()) == -1) {
            err_sys("fork error");
        } else if (childpid == 0) {         /* child process */
            if (close(listenfd) == -1) {    /* close listening socket */
                err_sys("close error");
            }
            str_echo(connfd);               /* process the request */
            exit(0);
        }
        if (close(connfd) == -1) {
            err_sys("close error");
        }
    }
}
