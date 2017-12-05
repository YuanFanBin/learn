#include <arpa/inet.h>      /* htonl, htons */
#include <netinet/in.h>     /* sockaddr_in */
#include <sys/socket.h>     /* socket, bind, listen */
#include <sys/select.h>     /* select */
#include <strings.h>        /* bzero */
#include <unistd.h>         /* read, write, close */
#include "error.h"

#define MAXLINE     4096    /* max text line length */
#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define LISTENQ     1024    /* 2nd argument to listen() */

/* 不派生子进程来处理链接，减少进程开销 */
// gcc tcpsrv_select01.c error.c error.h -o tcpsrv01
int main(int argc, char **argv)
{
    int                 i, maxi, maxfd, listenfd, connfd, sockfd;
    int                 nready, client[FD_SETSIZE];
    ssize_t             n;
    fd_set              rset, allset;
    char                buf[MAXLINE];
    socklen_t           clilen;
    struct sockaddr_in  cliaddr, servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        err_sys("bind error");
    }
    if (listen(listenfd, LISTENQ) < 0) {
        err_sys("listen error");
    }

    maxfd = listenfd;   /* initialize */
    maxi = -1;          /* index into client[] array */
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1; /* -1 indicated avaliable entry */
    }
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for ( ; ; ) {
        rset = allset;  /* structure assignment */
        if ((nready = select(maxfd, &rset, NULL, NULL, NULL)) < 0) {
            err_sys("select error");
        }

        if (FD_ISSET(listenfd, &rset)) {    /* new client connection */
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
                err_sys("accept error");
            }
            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = connfd; /* save description */
                    break;
                }
            }
            if (i == FD_SETSIZE) {
                err_quit("too many clients");
            }
            FD_SET(connfd, &allset);    /* add new description to set */
            if (connfd > maxfd) {
                maxfd = connfd;         /* for select */
            }
            if (i > maxi) {
                maxi = i;   /* max index in client[] array */
            }
            if (--nready <= 0) {
                continue;   /* no more readable description */
            }
        }
        for (i = 0; i <= maxi; i++) {   /* check all clients for data */
            if ((sockfd = client[i]) < 0) {
                continue;
            }
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                    /* connection closed by client */
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    if (write(sockfd, buf, n) != n) {
                        err_sys("write error");
                    }
                }
                if (--nready <= 0) {
                    break;  /* no more readable description */
                }
            }
        }
    }
}
