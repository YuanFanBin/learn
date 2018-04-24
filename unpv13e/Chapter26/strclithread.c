#include "../lib/error.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE     4096        /* max text line length */

static int   sockfd;
static FILE *fp;

void *copyto(void *arg);   // global for both threads to access
// ../Chapter03/readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);
// ../Chapter03/writen.c
ssize_t writen(int fd, const void *vptr, size_t n);

void str_cli(FILE *fp_arg, int sockfd_arg)
{
    char        recvline[MAXLINE];
    pthread_t   tid;
    int         n;

    sockfd = sockfd_arg;    // copy arguments to externals
    fp = fp_arg;

    if ((n = pthread_create(&tid, NULL, copyto, NULL)) != 0) {
        errno = n;
        err_sys("pthread_create error");
    }

    while (readline(sockfd, recvline, MAXLINE) > 0) {
        fputs(recvline, stdout);
    }
}

void *copyto(void *arg)
{
    char sendline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        writen(sockfd, sendline, strlen(sendline));
    }
    shutdown(sockfd, SHUT_WR);  // EOF on stdin, send FIN
    return(NULL);       // return (i.e., thread terminates) when EOF on stdin
}
