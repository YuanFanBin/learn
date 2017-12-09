#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */

char *Sock_ntop(const struct sockaddr *sa, socklen_t salen);
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int main(int argc, char **argv)
{
    int                     listenfd, connfd;
    socklen_t               len;
    char                    buf[MAXLINE];
    time_t                  ticks;
    struct sockaddr_storage cliaddr;

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], NULL);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], NULL);
    } else {
        fprintf(stderr, "usage: daytimetcpsrv2 [ <host> ] <service or port#>\n");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        len = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) == -1) {
            fprintf(stderr, "accept error: %s\n", strerror(errno));
            close(listenfd);
            exit(EXIT_FAILURE);
        }
        printf("connection from %s\n", Sock_ntop((struct sockaddr *) &cliaddr, len));

        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
        if (write(connfd, buf, strlen(buf)) == -1) {
            fprintf(stderr, "write error: %s\n", strerror(errno));
            close(listenfd);
            close(connfd);
            exit(EXIT_FAILURE);
        }
        close(connfd);
    }
    exit(EXIT_SUCCESS);
}

// $ sudo vim /etc/services # add services
// $ tail -n 2 /etc/services
// unpv13e         58888/tcp
// unpv13e         58888/udp
//
// # 服务端
// $ gcc daytimetcpsrv2.c tcp_listen.c ../lib/sock_ntop.c -o daytimetcpsrv2
// $ ./daytimetcpsrv2 192.168.1.42 unpv13e # 本机IPv4
// connection from 192.168.1.42:55062
//
// # 客户端
// $ nc -vt 192.168.1.42 unpv13e
// ArchLinux [192.168.1.42] 58888 (unpv13e) open
// Sat Dec  9 15:44:15 2017
