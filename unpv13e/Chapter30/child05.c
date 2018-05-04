#include "../lib/error.h"
#include "child.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

// 描述符传递式预先派生子进程服务器程序的child_make函数
pid_t child_make(int i, int listenfd, int addrlen)
{
    int     sockfd[2];
    pid_t   pid;
    void    child_main(int, int, int);

    if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0) {
        err_sys("socketpair error");
    }

    if ((pid = fork()) > 0) {
        close(sockfd[1]);
        cptr[i].child_pid = pid;
        cptr[i].child_pipefd = sockfd[0];
        cptr[i].child_status = 0;
        cptr[i].child_count = 0;
        return(pid);    // parent
    }
    dup2(sockfd[1], STDERR_FILENO); // child's stream pipe to parent
    close(sockfd[0]);
    close(sockfd[1]);
    close(listenfd);    // child does not need this open
    child_main(i, listenfd, addrlen);   // never returns
}

// ../Chapter15/read_fd.c
ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);

void child_main(int i, int listenfd, int addrlen)
{
    char    c;
    int     connfd;
    ssize_t n;
    void    web_child(int);

    printf("child %ld starting\n", (long) getpid());
    for ( ; ; ) {
        if ((n = read_fd(STDERR_FILENO, &c, 1, &connfd)) == 0) {
            err_quit("read_fd returned 0");
        }
        if (connfd < 0) {
            err_quit("no descriptor from read_fd");
        }

        web_child(connfd);  // process request
        close(connfd);

        write(STDERR_FILENO, "", 1);    // tell parent we're ready agagin
    }
}
