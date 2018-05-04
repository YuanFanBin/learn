#include "../lib/error.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 4096    /* max text line length */

int main(int argc, char **argv)
{
    int                 sockfd, n;
    char                recvline[MAXLINE + 1];
    struct sockaddr_in6 servaddr;

    if (argc != 2) {
        err_quit("usage: daytimetcpcli6 <IPaddress>");
    }

    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    /* /etc/services 中查看端口对应的服务 */
    servaddr.sin6_port = htons(13);      /* daytime server */
    if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0) {
        err_quit("inet_pton error for %s", argv[1]);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        err_sys("connect error, errno = %d", errno);
    }

    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;        /* null terminate */
        if (fputs(recvline, stdout) == EOF) {
            err_sys("fputs error");
        }
    }
    if (n < 0) {
        err_sys("read error");
    }
    exit(0);
}

/*
 * connection refused, errno = 111，不过理解大概意思了。
 *
 * 不足之处
 * 1. 需要用点分格式的IP地址做请求，无法用惯用名称请求；
 * 2. 这两个demo没有区分出协议无关性；
 */
