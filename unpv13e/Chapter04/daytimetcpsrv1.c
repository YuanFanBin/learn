#include <arpa/inet.h>  /* htonl, inet_ntop */
#include <errno.h>
#include <netinet/in.h> /* sockaddr_in */
#include <stdio.h>      /* snprintf */
#include <string.h>     /* strlen */
#include <strings.h>    /* bzero */
#include <sys/socket.h> /* socket, bind, listen, accept */
#include <time.h>       /* time, ctime */
#include <unistd.h>     /* write, close */
#include "error.h"      /* err_* */

#define MAXLINE 4096    /* max text line length */
#define LISTENQ 1024    /* 2nd argument to listen() */

/* gcc errno.h daytimetcpsrv1.c */
int main(int argc, char **argv)
{
    int                 err, n;
    int                 listenfd, connfd;
    socklen_t           len;
    struct sockaddr_in  servaddr, cliaddr;
    char                buff[MAXLINE];
    time_t              ticks;
    const char          *ptr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);  /* daytime server */

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
        len = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr,
                             &len)) < 0)
        {
#ifdef EPROTO
            if (errno == EPROTO || errno == ECONNABORTED) {
#else
            if (errno == ECONNABORTED) {
#endif
                goto again;
            } else {
                err_sys("accept error");
            }
        }
        if ((ptr = inet_ntop(AF_INET, &cliaddr.sin_addr, buff,
                             sizeof(buff))) == NULL)
        {
            err_sys("inet_ntop error"); /* sets errno */
        }
        printf("connection from %s, port %d\n", ptr, ntohs(cliaddr.sin_port));
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        if ((n = write(connfd, buff, strlen(buff))) != strlen(buff)) {
            err_sys("write error");
        }
        close(connfd);
    }
}
