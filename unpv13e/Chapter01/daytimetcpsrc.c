#include <stdio.h>      /* snprintf */
#include <string.h>     /* strlen */
#include <arpa/inet.h>  /* htonl */
#include <netinet/in.h> /* sockaddr_in */
#include <strings.h>    /* bzero */
#include <sys/socket.h> /* socket, bind, listen, accept */
#include <time.h>       /* time, ctime */
#include <unistd.h>     /* write, close */
#include "error.h"      /* err_* */

#define MAXLINE 4096    /* max text line length */
#define LISTENQ 1024    /* 2nd argument to listen() */

/* figure-1.9 */
/* $ gcc error.h daytimetcpsrc.c */
int main(int argc, char **argv)
{
    int                 err, n;
    int                 listenfd, connfd;
    struct sockaddr_in  servaddr;
    char                buff[MAXLINE];
    time_t              ticks;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* /etc/services */
    servaddr.sin_port = htons(13);      /* daytime server */

    if ((err = bind(listenfd, (struct sockaddr *) &servaddr,
                    sizeof(servaddr))) < 0) {
        err_sys("bind error");
    }
    //if ((err = listen(listenfd, 2)) < 0) {        # 1
    if ((err = listen(listenfd, LISTENQ)) < 0) {  //  # 0
        err_sys("listen error");
    }

    for ( ; ; ) {
again:
        if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) < 0) {
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
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        /* 忽略2个strlen问题 */
        if ((n = write(connfd, buff, strlen(buff))) != strlen(buff)) {
            err_sys("write error");
        }
        //sleep(3);                                 # 2
        close(connfd);
    }
}

/*
 * 注释 #1, 打开 #2, #3，没有理解LISTENQ的用法，后续章节有介绍，再细看吧。
 */
