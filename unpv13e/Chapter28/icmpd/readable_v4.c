#include "icmpd.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

// ../../lib/sock_ntop_host.c
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

// 处理所接收到的ICMPv4数据报
int readable_v4(void)
{
    int                 i, hlen1, hlen2, icmplen, sport;
    char                buf[MAXLINE];
    char                srcstr[INET_ADDRSTRLEN], dststr[INET_ADDRSTRLEN];
    ssize_t             n;
    socklen_t           len;
    struct ip          *ip, *hip;
    struct icmp        *icmp;
    struct udphdr      *udp;
    struct sockaddr_in  from, dest;
    struct icmpd_err    icmpd_err;

    len = sizeof(from);
    if ((n = recvfrom(fd4, buf, MAXLINE, 0, (struct sockaddr *) &from, &len)) < 0) {
        err_sys("recvfrom error");
    }
    printf("%ld bytes ICMPv4 from %s: ", n, sock_ntop_host((struct sockaddr *) &from, len));
    ip = (struct ip *) buf; // start of IP header
    hlen1 = ip->ip_hl << 2; // length of IP header
    icmp = (struct icmp *) (buf + hlen1);   // start of ICMP header
    if ((icmplen = n - hlen1) < 8) {
        err_quit("icmplen (%d) < 8", icmplen);
    }
    printf(" type = %d, code = %d\n", icmp->icmp_type, icmp->icmp_code);

    // 目的地不可达，超时，源熄灭 ICMPv4消息
    if (icmp->icmp_type == ICMP_UNREACH ||
        icmp->icmp_type == ICMP_TIMXCEED ||
        icmp->icmp_type == ICMP_SOURCEQUENCH)
    {
        if (icmplen < 8 + 20 + 8) {
            err_quit("icmplen (%d) < 8 + 20 + 8", icmplen);
        }

        hip = (struct ip *) (buf + hlen1 + 8);
        hlen2 = hip->ip_hl << 2;
        printf("\tsrcip = %s, dstip = %s, proto = %d\n",
                inet_ntop(AF_INET, &hip->ip_src, srcstr, sizeof(srcstr)),
                inet_ntop(AF_INET, &hip->ip_dst, dststr, sizeof(dststr)),
                hip->ip_p);
        // 检查UDP出错信息，找出相应客户
        if (hip->ip_p == IPPROTO_UDP) {
            udp = (struct udphdr *) (buf + hlen1 + 8 + hlen2);
            sport = udp->uh_sport;

            // find client's Unix domain socket, send headers
            for (i = 0; i <= maxi; i++) {
                if (client[i].connfd >= 0 && client[i].family == AF_INET && client[i].lport == sport) {
                    // 构造一个IPv4套接字地址结构，存放引发错本错误的那个UDP数据啊薄的谜底IP地址和目的端口号
                    bzero(&dest, sizeof(dest));
                    dest.sin_family = AF_INET;
#ifdef HAVE_SOCKADDR_SA_LEN
                    dest.sin_len = sizeof(dest)
#endif
                    memcpy(&dest.sin_addr, &hip->ip_dst, sizeof(struct in_addr));
                    dest.sin_port = udp->uh_dport;

                    // 构造icmpd_err结构
                    icmpd_err.icmpd_type = icmp->icmp_type;
                    icmpd_err.icmpd_code = icmp->icmp_code;
                    icmpd_err.icmpd_len = sizeof(struct sockaddr_in);
                    memcpy(&icmpd_err.icmpd_dest, &dest, sizeof(dest));

                    // convert type & code to reasonable errno value
                    icmpd_err.icmpd_errno = EHOSTUNREACH;   // default
                    if (icmp->icmp_type == ICMP_UNREACH) {
                        if (icmp->icmp_code == ICMP_UNREACH_PORT) {
                            icmpd_err.icmpd_errno = ECONNREFUSED;
                        } else if (icmp->icmp_code == ICMP_UNREACH_NEEDFRAG) {
                            icmpd_err.icmpd_errno = EMSGSIZE;
                        }
                    }
                    write(client[i].connfd, &icmpd_err, sizeof(icmpd_err));
                }
            }
        }
    }
    return(--nready);
}
