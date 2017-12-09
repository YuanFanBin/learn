#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */

char *Sock_ntop(const struct sockaddr *sa, socklen_t salen);
int udp_server(const char *host, const char *serv, socklen_t *addrlenp);

int main(int argc, char **argv)
{
    int                     sockfd;
    ssize_t                 n;
    char                    buff[MAXLINE];
    time_t                  ticks;
    socklen_t               len;
    struct sockaddr_storage cliaddr;

    if (argc == 2) {
        sockfd = udp_server(NULL, argv[1], NULL);
    } else if (argc == 3) {
        sockfd = udp_server(argv[1], argv[2], NULL);
    } else {
        fprintf(stderr, "usage: daytimeudpsrv2 [ <host> ] <service or port#>\n");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        len = sizeof(cliaddr);
        if ((n = recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len)) == -1) {
            fprintf(stderr, "recvfrom error: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
        printf("datagram from %s\n", Sock_ntop((struct sockaddr *) &cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        if (sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &cliaddr, len) == -1) {
            fprintf(stderr, "sendto error: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}

// $ sudo vim /etc/services # add services
// $ tail -n 2 /etc/services
// unpv13e         58888/tcp
// unpv13e         58888/udp
//
// # 服务端
// $ gcc daytimeudpsrv2.c udp_server.c ../lib/sock_ntop.c -o daytimeudpsrv2
// $ ./daytimeudpsrv2 localhost unpv13e
// datagram from sock_ntop: unknown AF_xxx: 10, len 28      # 不知道为什么是 unknown
//
// # 客户端
// $ gcc daytimeudpcli1.c udp_client.c ../lib/sock_ntop_host.c -o daytimeudpcli1
// $ ./daytimeudpcli1 localhost unpv13e
// ArchLinux [192.168.1.42] 58888 (unpv13e) open
// Sat Dec  9 15:44:15 2017
