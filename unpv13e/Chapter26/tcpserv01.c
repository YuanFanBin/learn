#include "../lib/error.h"
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static void *doit(void *arg);  // each thread executes this function
// ../Chapter11/tcp_listen.c
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);
// ../Chapter05/str_echo.c
void str_echo(int sockfd);

int main(int argc, char **argv)
{
    int              listenfd, connfd, n;
    pthread_t        tid;
    socklen_t        addrlen, len;
    struct sockaddr *cliaddr;

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: tcpserv01 [ <host> ] <service or port>");
    }

    cliaddr = (struct sockaddr *) malloc(addrlen);

    for ( ; ; ) {
        len = addrlen;
        // 可能存在问题：多线程执行时connfd指向了同一个内存空间
        connfd = accept(listenfd, cliaddr, &len);
        if ((n = pthread_create(&tid, NULL, &doit, (void *) connfd)) != 0) {
            errno = n;
            err_sys("pthread_create error");
        }
    }
}

static void *doit(void *arg)
{
    int n;
    if ((n = pthread_detach(pthread_self())) != 0) {
        errno = n;
        err_sys("pthread_detach error");
    }
    str_echo((int)arg);
    close((int)arg);
    return(NULL);
}
