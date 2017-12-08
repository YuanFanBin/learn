#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <strings.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../lib/error.h"

#define MAXLINE 4096 /* max text line length */
#define SERV_PORT 9877 /* TCP and UDP client-servers */
#define LISTENQ 1024

#define max(a, b)   ((a) > (b) ? (a) : (b))

void str_echo(int sockfd); // Chapter05 str_echo[str_echo.c, str_echo08.c, str_echo09.c]

int main(int argc, char **argv) {
    int         listenfd, connfd, udpfd, nready, maxfdp1;
    char        mesg[MAXLINE];
    pid_t       childpid;
    fd_set      rset;
    ssize_t     n;
    socklen_t   len;
    const int   on = 1;
    void        sig_chld(int);
    struct sockaddr_in cliaddr , servaddr;

    // 1. create listening TCP socket
    // socket -> (setsockopt) -> bind -> listen
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        err_sys("tcp socket error");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    // 设置 SO_REUSEADDR 套接字选项以防止该端口上已有连接存在
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        err_sys("tcp setsockopt error");
    }
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        err_sys("tcp bind error");
    }
    if (listen(listenfd, LISTENQ) == -1) {
        err_sys("tcp listen error");
    }

    // 2. create UDP socket
    // socket -> bind
    if((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        err_sys("udp socket error");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // TCP端口与UDP端口相互独立
    servaddr.sin_port = htons(SERV_PORT);
    if (bind(udpfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        err_sys("udp bind error");
    }

    // 调用子进程来处理TCP连接，需要接受子进程终止信号
    if (signal(SIGCHLD, sig_chld)) { // waitpid
        err_sys("signal error");
    }
    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;
    for (;;) {
        // 每次循环均要重新设置fd_set
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);
        if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                err_sys("select error");
            }
        }
        if (FD_ISSET(listenfd, &rset)) { // TCP
            len = sizeof(cliaddr);
            if((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) == -1) {
                err_sys("tcp accept error");
            }
            if ((childpid = fork()) == 0) { // child
                close(listenfd); // 子进程关闭listenfd
                str_echo(connfd);
                exit(0);
            }
            close(connfd); // 父进程关闭connfd
        }
        if (FD_ISSET(udpfd, &rset)) { // UDP
            len = sizeof(cliaddr);
            if ((n = recvfrom(udpfd, mesg, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len)) == -1) {
                err_sys("udp recvfrom error");
            }
            if (sendto(udpfd, mesg, n, 0, (struct sockaddr *) &cliaddr, len) == -1) {
                err_sys("udp sendto error");
            }
        }
    }
}

// $ gcc udpserv_select01.c ../lib/error.c ../lib/error.h ../Chapter05/str_echo09.c -o udpsrv
