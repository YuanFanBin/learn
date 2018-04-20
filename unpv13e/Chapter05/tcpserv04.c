#include "../lib/error.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define LISTENQ     1024    /* 2nd argument to listen() */
#define MAXLINE     4096    /* max text line length */

void str_echo(int sockfd);
void sig_chld(int signo);

int main(int argc, char **argv)
{
    int                 err;
    int                 listenfd, connfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_in  cliaddr, servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ((err = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
        err_sys("bind error");
    }

    if ((err = listen(listenfd, LISTENQ)) < 0) {
        err_sys("listen error");
    }

    signal(SIGCHLD, sig_chld);

    for ( ; ; ) {
again:
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
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
        if (close(connfd) == -1) {          /* parent closes connected socket */
            err_sys("close error");
        }
    }
}

/*
 * 网络编程可能遇到的三种情况：
 * （1）当fork子进程时，必须捕获SIGCHLD信号；
 * （2）当捕获信号时，必须处理被中断的系统调用；
 * （3）SIGCHLD的信号处理函数必须正确编写，应使用waitpid函数以避免留下僵死进程。
 */
