#include "../Chapter17/unpifi.h"
#include "../lib/error.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT       9877    /* TCP and UDP */
#define MAXLINE         4096    /* max text line length */

// ../lib/sock_ntop.c
char *sock_ntop(const struct sockaddr *sa, socklen_t salen);
void mydg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen, struct sockaddr *myaddr);

int main(int argc, char **argv)
{
    int                  sockfd;
    const int            on = 1;
    pid_t                pid;
    struct ifi_info     *ifi; // , *ifihead;
    struct sockaddr_in  *sa, cliaddr, wildaddr;

    for (/* ifihead = */ifi = get_ifi_info(AF_INET, 1); ifi != NULL; ifi = ifi->ifi_next) {
        // 绑定单播地址
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
            err_sys("setsockopt error");
        }

        sa = (struct sockaddr_in *) ifi->ifi_addr;
        sa->sin_family = AF_INET;
        sa->sin_port = htons(SERV_PORT);
        if (bind(sockfd, (struct sockaddr *) sa, sizeof(*sa)) < 0) {
            err_sys("bind error");
        }
        printf("bound %s\n", sock_ntop((struct sockaddr *) sa, sizeof(*sa)));

        if ((pid = fork()) == 0) { //child
            // 子进程相应单播echo
            mydg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr), (struct sockaddr *) sa);
            exit(0);    // never executed
        }
        if (ifi->ifi_flags & IFF_BROADCAST) {
            // 绑定广播地址
            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
                err_sys("setsockopt error");
            }

            sa = (struct sockaddr_in *) ifi->ifi_brdaddr;
            sa->sin_family = AF_INET;
            sa->sin_port = htons(SERV_PORT);
            if (bind(sockfd, (struct sockaddr *) sa, sizeof(*sa)) < 0) {
                if (errno == EADDRINUSE) {
                    printf("EADDRINUSE: %s\n", sock_ntop((struct sockaddr *) sa, sizeof(*sa)));
                    close(sockfd);
                    continue;
                }
                err_sys("bind error for %s", sock_ntop((struct sockaddr *) sa, sizeof(*sa)));
            }
            printf("bound %s\n", sock_ntop((struct sockaddr *) sa, sizeof(*sa)));

            if ((pid = fork()) == 0) { // child
                // 子进程相应广播echo
                mydg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr), (struct sockaddr *) sa);
                exit(0);    // never executed
            }
        }
    }
    // 绑定通配地址
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        err_sys("setsockopt error");
    }

    bzero(&wildaddr, sizeof(wildaddr));
    wildaddr.sin_family = AF_INET;
    wildaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    wildaddr.sin_port = htons(SERV_PORT);
    if (bind(sockfd, (struct sockaddr *) &wildaddr, sizeof(wildaddr)) < 0) {
        err_sys("bind error");
    }
    printf("bound %s\n", sock_ntop((struct sockaddr *) &wildaddr, sizeof(wildaddr)));

    if ((pid = fork()) == 0) { // child
        // 子进程通配地址的响应
        mydg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr), (struct sockaddr *) sa); // sa ?? un init
        exit(0);    // never executed
    }
    exit(0);
}

void mydg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen, struct sockaddr *myaddr)
{
    int         n;
    char        mesg[MAXLINE];
    socklen_t   len;

    for ( ; ; ) {
        len = clilen;
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        printf("child %d, datagram from %s", getpid(), sock_ntop(pcliaddr, len));
        printf(", to %s\n", sock_ntop(myaddr, clilen));
        sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}
