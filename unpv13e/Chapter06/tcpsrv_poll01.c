#include "../lib/error.h"
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <errno.h>
#include <limits.h>
#include <poll.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE     4096    /* max text line length */
#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define LISTENQ     1024    /* 2nd argument to listen() */

#ifndef OPEN_MAX    /* 已被移除 */
#define OPEN_MAX    1024
#endif

#ifndef INFTIM      /* 已被移除 */
#define INFTIM      (-1)
#endif

/* gcc tcpsrv_poll01.c error.c -o srv01 */
int main(int argc, char **argv)
{
    int                 i, maxi, listenfd, connfd, sockfd;
    int                 nready;
    ssize_t             n;
    char                buf[MAXLINE];
    socklen_t           clilen;
    struct pollfd       client[OPEN_MAX];
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

    client[0].fd = listenfd;
    client[0].events = POLLIN; // TODO: 未定义POLLRDNORM;
    for (i = 0; i < OPEN_MAX; i++) {
        client[i].fd = -1;          /* -1 indicates avaliable entry */
    }
    maxi = 0;                       /* max index into client[] array */

    for ( ; ; ) {
        if ((nready = poll(client, maxi + 1, INFTIM)) < 0) {
            err_sys("poll error");
        }

        if (client[0].revents & POLLIN /* TODO: POLLRDNORM */ ) {    /* new client connection */
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr,
                                 &clilen)) < 0)
            {
                err_sys("accept error");
            }
            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;      /* save descriptor */
                    break;
                }
            }
            if (i == OPEN_MAX) {
                err_quit("too many clients");
            }
            client[i].events = POLLIN /* TODO: POLLRDNORM*/ ;
            if (i > maxi) {
                maxi = i;       /* max index in client[] array */
            }
            if (--nready <= 0) {
                continue;       /* no more readable description */
            }
        }
        for (i = 1; i <= maxi; i++) {   /* check all client[] array */
            if ((sockfd = client[i].fd) < 0) {
                continue;
            }
            if (client[i].revents & (POLLIN /* TODO: POLLRDNORM */ | POLLERR)) {
                if ((n = read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        /* connection reset by client */
                        close(sockfd);
                        client[i].fd = -1;
                    } else {
                        err_sys("read error");
                    }
                } else if (n == 0) {
                    /* connection closed by client */
                    close(sockfd);
                    client[i].fd = -1;
                } else {
                    if (write(sockfd, buf, n) != n) {
                        err_sys("write error");
                    }
                }
                if (--nready <= 0) {
                    break;      /* no more readable descriptors */
                }
            }
        }
    }
}
