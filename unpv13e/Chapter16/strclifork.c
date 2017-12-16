#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "../lib/error.h"

#define MAXLINE     4096        /* max text line length */

// Chapter03/readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);

void str_cli(FILE *fp, int sockfd)
{
    pid_t pid;
    char sendline[MAXLINE], recvline[MAXLINE];

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { //child
        while (readline(sockfd, recvline, MAXLINE) > 0) {
            fputs(recvline, stdout);
        }
        kill(getppid(), SIGTERM); // in case parent still running
        exit(0);
    }
    // parent: stdin -> server
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        write(sockfd, sendline, strlen(sendline));
    }
    shutdown(sockfd, SHUT_WR); // 这里不能使用close，close之后子进程可能无法从sockfd读取数据，数据可能丢失。
    pause();
    return;
}
