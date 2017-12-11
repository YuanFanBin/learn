#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "../lib/error.h"

#define MAXLINE 4096 /* max text line length */

int daemon_init(const char *pname, int facility);
char *Sock_ntop(const struct sockaddr *sa, socklen_t salen);
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int main(int argc, char **argv)
{
    int              listenfd, connfd;
    socklen_t        addrlen, len;
    struct sockaddr *cliaddr;
    char             buf[MAXLINE];
    time_t           ticks;

    if (argc < 2 || argc > 3) {
        err_quit("usage: daytimetcpsrv2 [ <host> ] <service or port>");
    }

    daemon_init(argv[0], 0);

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    }

    cliaddr = (struct sockaddr *) malloc (addrlen);

    for (;;) {
        len = addrlen;
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) == -1) {
            err_sys("accept error");
        }
        err_msg("connection from %s\n", Sock_ntop((struct sockaddr *) &cliaddr, len));

        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
        if (write(connfd, buf, strlen(buf)) == -1) {
            err_sys("write error");
        }
        close(connfd);
    }
}

// $ sudo vim /etc/services # add services
// $ tail -n 2 /etc/services
// unpv13e         58888/tcp
// unpv13e         58888/udp
//
// # 服务端
// $ gcc daytimetcpsrv2.c daemon_init.c ../Chapter11/tcp_listen.c ../lib/sock_ntop.c ../lib/error.c -o daytimetcpsrv2
// $ ./daytimetcpsrv2 192.168.1.42 unpv13e
// $ ps aux | grep daytimetcpsrv2
// fanbin    3129  0.0  0.0   6400  1556 ?        S    14:37   0:00 ./daytimetcpsrv2 192.168.1.42 unpv13e
//
// # 客户端
// $ nc -vt 192.168.1.42 unpv13e
// $ ArchLinux [192.168.1.42] 58888 (unpv13e) open
// $ Mon Dec 11 14:53:19 2017
//
// # syslogd 日志
// $ strings user-1000.journal | tail -n 10
// ~hHX9
// 3xq?
// r`a[p
// MESSAGE=connection from 192.168.1.42:35810      # here
// _SOURCE_REALTIME_TIMESTAMP=1512976247709692
// 1'_>q)
// )`V9
// ~hHX9
// 3xq?
// r`a[p
