#include "../Chapter17/unpifi.h"
#include "../lib/error.h"
#include "pktinfo.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

#undef MAXLINE
#define MAXLINE             20      // to see datagram truncation
#define INET6_ADDRSTRLEN    46

#ifndef IFNAMSIZ
#define IFNAMSIZ            16
#endif

// ../lib/sock_ntop.c
char *sock_ntop(const struct sockaddr *sa, socklen_t salen);
// recvfromflags.c
ssize_t recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp, struct sockaddr *sa, socklen_t *salenptr, struct unp_in_pktinfo *pktp);

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int                     flags;
    const int               on = 1;
    socklen_t               len;
    ssize_t                 n;
    char                    mesg[MAXLINE], str[INET6_ADDRSTRLEN], ifname[IFNAMSIZ];
    struct in_addr          in_zero;
    struct unp_in_pktinfo   pktinfo;

#ifdef IP_RECVDSTADDR // 返回目的IP地址
    if (setsockopt(sockfd, IPPROTO_IP, IP_RECVDSTADDR, &on, sizeof(on)) < 0) {
        err_ret("setsockopt of IP_RECVDSTADDR");
    }
#endif
#ifdef IP_RECVIF // 返回接收接口索引
    if (setsockopt(sockfd, IPPROTO_IP, IP_RECVIF, &on, sizeof(on)) < 0) {
        err_ret("setsockopt of IP_RECVIF");
    }
#endif
    bzero(&in_zero, sizeof(struct in_addr));    // all 0 IPv4 address

    for ( ; ; ) {
        len = clilen;
        flags = 0;
        n = recvfrom_flags(sockfd, mesg, MAXLINE, &flags, pcliaddr, &len, &pktinfo);
        printf("%ld-byte datagram from %s", n, sock_ntop(pcliaddr, len));
        if (memcmp(&pktinfo.ipi_addr, &in_zero, sizeof(in_zero)) != 0) {
            printf(", to %s", inet_ntop(AF_INET, &pktinfo.ipi_addr, str, sizeof(str)));
        }
        if (pktinfo.ipi_ifindex > 0) {
            printf(", recv i/f = %s", if_indextoname(pktinfo.ipi_ifindex, ifname));
        }
#ifdef MSG_TRUNC        // 4.3BSD Reno and later
        if (flags & MSG_TRUNC) {
            printf(" (datagram truncated)");
        }
#endif
#ifdef MSG_CTRUNC
        if (flags & MSG_CTRUNC) {
            printf(" (control info truncated)");
        }
#endif
#ifdef MSG_BCAST
        if (flags & MSG_BCAST) {
            printf(" (broadcast)");
        }
#endif
#ifdef MSG_MCAST
        if (flags & MSG_MCAST) {
            printf(" (multicast)");
        }
#endif
        printf("\n");
        sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}
