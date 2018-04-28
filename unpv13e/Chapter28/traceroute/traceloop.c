#include "trace.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

// ../../lib/sock_ntop_host.c
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
// sock_set_port.c
void sock_set_port(struct sockaddr *sa, socklen_t salen, int port);
// sock_cmp_addr.c
int sock_cmp_addr(const struct sockaddr *sa1, const struct sockaddr *sa2, socklen_t salen);

void traceloop(void)
{
    int                  seq, code, done;
    double               rtt;
    struct rec          *rec;
    struct timeval       tvrecv;
    char                 str[NI_MAXHOST];
#ifdef IPV6
    struct icmp6_filter  myfilt;
#endif

    // 读入所有返送ICMP消息的一个原始套接字
    if ((recvfd = socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto)) < 0) {
        err_sys("socket error");
    }
    setuid(getuid());   // don't need special permissions anymore

#ifdef IPV6
    if (pr->sasend->sa_family == AF_INET6 && verbose == 0) {
        ICMP6_FILTER_SETBLOCKALL(&myfilt);
        ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &myfilt);
        ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &myfilt);
        setsockopt(recvfd, IPPROTO_IPV6, ICMP6_FILTER, &myfilt, sizeof(myfilt));
    }
#endif

    // 以不断递增的TTL写出探测分组的一个UDP套接字
    if ((sendfd = socket(pr->sasend->sa_family, SOCK_DGRAM, 0)) < 0) {
        err_sys("socket error");
    }

    // 本程序可能有多个副本同时运行在本地主机上，我们有必要区分一个接收ICMP消息
    // 是处于响应本进程发送的数据报产生的还是出于响应其他 traceroute 进程发送的
    // 数据报产生的。
    pr->sabind->sa_family = pr->sasend->sa_family;
    sport = (getpid() & 0xffff) | 0x8000;   // our source UDP port #
    sock_set_port(pr->sabind, pr->salen, htons(sport));
    if (bind(sendfd, pr->sabind, pr->salen) < 0) {
        err_sys("socket error");
    }

    sig_alrm(SIGALRM);

    seq = 0;
    done = 0;
    // TTL 跳限
    for (ttl = 1; ttl <= max_ttl && done == 0; ttl++) {
        if (setsockopt(sendfd, pr->ttllevel, pr->ttloptname, &ttl, sizeof(int)) < 0) {
            err_sys("setsockopt error");
        }
        bzero(pr->salast, pr->salen);

        printf("%2d ", ttl);
        fflush(stdout);

        // 对每个跳限发送3个探测分组
        for (probe = 0; probe < nprobes; probe++) {
            rec = (struct rec *) sendbuf;
            rec->rec_seq = ++seq;
            rec->rec_ttl = ttl;
            gettimeofday(&rec->rec_tv, NULL);

            // 每个探测分组发往不同的端口中
            sock_set_port(pr->sasend, pr->salen, htons(dport + seq));
            sendto(sendfd, sendbuf, datalen, 0, pr->sasend, pr->salen);

            if ((code = (*pr->recv)(seq, &tvrecv)) == -3) { // -3: 探测分组
                printf(" *");       // timeout, no reply
            } else {
                if (sock_cmp_addr(pr->sarecv, pr->salast, pr->salen) != 0) {
                    if (getnameinfo(pr->sarecv, pr->salen, str, sizeof(str), NULL, 0, 0) == 0) {
                        printf(" %s (%s)", str, sock_ntop_host(pr->sarecv, pr->salen));
                    } else {
                        printf(" %s", sock_ntop_host(pr->sarecv, pr->salen));
                    }
                    memcpy(pr->salast, pr->sarecv, pr->salen);
                }
                tv_sub(&tvrecv, &rec->rec_tv);
                rtt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
                printf("  %.3f ms", rtt);
                if (code == -1) {   // port unreachable; at destination
                    done++;
                } else if (code >= 0) {
                    printf("  (ICMP %s)", (*pr->icmpcode(code)));
                }
            }
            fflush(stdout);
        }
        printf("\n");
    }
}
