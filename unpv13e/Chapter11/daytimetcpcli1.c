#include "../lib/error.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */

char *Sock_ntop(const struct sockaddr *sa, socklen_t salen);

int main(int argc, char **argv) {
    int                  sockfd, n;
    char                 recvline[MAXLINE + 1];
    struct sockaddr_in   servaddr;
    struct in_addr     **pptr;
    struct in_addr      *inetaddrp[2];
    struct in_addr       inetaddr;
    struct hostent       *hp;
    struct servent       *sp;

    if (argc != 3) {
        err_quit("usage: daytimedaycli1 <hostname> <service>");
    }
    // 通过主机或IP地址获取 struct in_addr 结构数据
    if ((hp = gethostbyname(argv[1])) == NULL) {
        // a.b.c.d
        if (inet_aton(argv[1], &inetaddr) == 0) {
            err_quit("hostname error for %s: %s", argv[1], hstrerror(h_errno));
        } else {
            inetaddrp[0] = &inetaddr;
            inetaddrp[1] = NULL;
            pptr = inetaddrp;
        }
    } else {
        pptr = (struct in_addr **) hp->h_addr_list;
    }

    // 根据服务名称查找对应端口（找到的端口为网络字节序，无需再次转换）
    if ((sp = getservbyname(argv[2], "tcp")) == NULL) {
        err_quit("getservbyname error for %s", argv[2]);
    }

    // 从已知的主机列表中绑定一个TCP连接
    for (; *pptr != NULL; pptr++) {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            err_sys("socket error");
        }
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = sp->s_port;
        memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
        printf("trying %s\n", Sock_ntop((struct sockaddr *) &servaddr, sizeof(servaddr)));

        if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == 0) {
            break; /* success */
        }
        err_ret("connect error");
        close(sockfd);
    }
    if (*pptr == NULL) {
        err_quit("unable to connect");
    }
    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0; /* null terminate */
        fputs(recvline, stdout);
    }
}

// $ gcc daytimetcpcli1.c ../lib/error.h ../lib/error.c ../lib/sock_ntop.c -o daytimetcpcli1
