#include "icmpd.h"

int main(int argc, char **argv)
{
    int                 i, sockfd;
    struct sockaddr_un  sun;

    if (argc != 1) {
        err_quit("usage: icmpd");
    }

    maxi = -1;  // index into client[] array
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i].connfd = -1;  // -1 indicates available entry
    }
    FD_ZERO(&allset);

    // 原始ICMPv4套接字
    if ((fd4 = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        err_sys("socket error");
    }
    FD_SET(fd4, &allset);
    maxfd = fd4;

#ifdef IPV6
    // 原始ICMPv6套接字
    if ((fd6 = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6)) < 0) {
        err_sys("socket error");
    }
    maxfd = max(maxfd, fd6);
#endif

    // Unix域字节流套接字
    if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        err_sys("listenfd error");
    }
    sun.sun_family = AF_LOCAL;
    strcpy(sun.sun_path, ICMPD_PATH);
    unlink(ICMPD_PATH);
    if (bind(listenfd, (struct sockaddr *) &sun, sizeof(sun)) < 0) {
        err_sys("bind error");
    }
    if (listen(listenfd, LISTENQ) < 0) {
        err_sys("listen error");
    }
    FD_SET(listenfd, &allset);
    maxfd = max(maxfd, listenfd);

    for ( ; ; ) {
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) {
            if (readable_listen() <= 0) {
                continue;
            }
        }

        if (FD_ISSET(fd4, &rset)) {
            if (readable_v4() <= 0) {
                continue;
            }
        }

#ifdef IPV6
        if (FD_ISSET(fd6, &rset)) {
            if (readable_v6() <= 0) {
                continue;
            }
        }
#endif

        for (i = 0; i <= maxi; i++) {   // check all clients for data
            if ((sockfd = client[i].connfd) < 0) {
                continue;
            }
            if (FD_ISSET(sockfd, &rset)) {
                if (readable_conn(i) <= 0) {
                    break;  // no more readable descriptors
                }
            }
        }
    }
    exit(0);
}
