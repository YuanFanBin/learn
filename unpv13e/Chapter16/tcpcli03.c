#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../lib/error.h"

#define SERV_PORT   9877    /* TCP and UDP client-servers */

int main(int argc, char **argv)
{
    int                 n, sockfd;
    struct linger       ling;
    struct sockaddr_in  servaddr;

    if (argc != 2) {
        err_quit("usage: tcpcli03 <IPaddress>");
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if ((n = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) < 0) {
        err_sys("inet_pton error for %s", argv[1]);
    } else if (n == 0) {
        err_quit("inet_pton error for %s", argv[1]);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        err_sys("connect error, errno = %d", errno);
    }

    // 详细说明参考7.5小节内容
    ling.l_onoff = 1; /* cause RST to be sent on close() */
    ling.l_linger = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0) {
        err_sys("setsockopt error");
    }
    close(sockfd);
    exit(0);
}

// # 客户端
// $ gcc tcpcli03.c ../lib/error.c -o tcpcli03
