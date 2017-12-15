#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "../lib/error.h"

#define SERV_PORT   9877    /* TCP and UDP client-servers */

void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
    int                 i, n, sockfd[5];
    struct sockaddr_in  servaddr;

    if (argc != 2) {
        err_quit("usage: tcpcli <IPaddress>");
    }

    for (i = 0; i < 5; i++) {
        if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
        if (connect(sockfd[i], (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
            err_sys("connect error, errno = %d", errno);
        }
    }
    str_cli(stdin, sockfd[0]); /* do it all */
    // 5个客户端连接同时终止
    exit(0);
}

// # 服务端：打开tcpserv01.c #2
// $ gcc tcpserv01.c str_echo.c ../lib/error.c sigchildwait.c -o tcpserv03
// $ ./tcpserv03
// child 26487 terminated
// child 26488 terminated
//
// # 客户端
// $ gcc tcpcli04.c str_cli.c ../lib/error.c ../Chapter03/readline.c -o tcpcli04
// $ ./tcpcli04 127.0.0.1
// hello
// hello
// ^C
//
// # 进程状态
// $ ps aux | grep tcpserv03
// fanbin   26286  0.0  0.0   4276   824 pts/7    S+   15:16   0:00 ./tcpserv03
// fanbin   26489  0.0  0.0      0     0 pts/7    Z+   15:16   0:00 [tcpserv03] <defunct>
// fanbin   26490  0.0  0.0      0     0 pts/7    Z+   15:16   0:00 [tcpserv03] <defunct>
// fanbin   26491  0.0  0.0      0     0 pts/7    Z+   15:16   0:00 [tcpserv03] <defunct>
