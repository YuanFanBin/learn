#include "../lib/error.h"
#include "unproute.h"
#include <arpa/inet.h>
#include <net/route.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFLEN  (sizeof(struct rt_msghdr) + 512)
                /* sizeof (struct sockaddr_in6) * 8 = 192 */
#define SEQ     9999

// ../lib/sock_ntop_host.c
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

int main(int argc, char **argv)
{
    int                 sockfd;
    char               *buf;
    pid_t               pid;
    ssize_t             n;
    struct rt_msghdr   *rtm;
    struct sockaddr    *sa, *rti_info[RTAX_MAX];
    struct sockaddr_in *sin;

    if (argc != 2) {
        err_quit("usage: getrt <IPaddress>");
    }
    sockfd = socket(AF_ROUTE, SOCK_RAW, 0); // 需要超级权限

    buf = calloc(1, BUFLEN);

    // NOTE: figure-18.5
    // 送往内核的结构 rt_msghdr
    rtm = (struct rt_msghdr *) buf;
    rtm->rtm_msglen = sizeof(struct rt_msghdr) + sizeof(struct sockaddr_in);
    rtm->rtm_version = RTM_VERSION;
    rtm->rtm_type = RTM_GET;
    rtm->rtm_addrs = RTA_GET;
    rtm->rtm_pid = pid = getpid();
    rtm->rtm_seq = SEQ;

    // 送往内核的结构 目的套接字结构 RTA_DST
    sin = (struct sockaddr_in *) (rtm + 1);
    sin->sin_len = sizeof(struct sockaddr_in);
    sin->sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &sin->sin_addr);

    // 发往内核
    write(sockfd, rtm, rtm->rtm_msglen);

    // 从内核中读取响应结果
    do {
        n = read(sockfd, rtm, BUFLEN);
    } while (rtm->rtm_type != RTM_GET || rtm->rtm_seq != SEQ || rtm->rtm_pid != pid);

    // NOTE: figure-18.8
    rtm = (struct rt_msghdr *) buf;
    sa = (struct sockaddr *) (rtm + 1);
    get_rtaddrs(rtm->rtm_addrs, sa, rti_info);
    if ((sa = rti_info[RTAX_DST]) != NULL) {
        printf("dest: %s\n", sock_ntop_host(sa, sa->sa_len));
    }

    if ((sa = rti_info[RTAX_GATEWAY]) != NULL) {
        printf("gateway: %s\n", sock_ntop_host(sa, sa->sa_len));
    }

    if ((sa = rti_info[RTAX_NETMASK]) != NULL) {
        printf("netmask: %s\n", sock_masktop(sa, sa->sa_len));
    }

    if ((sa = rti_info[RTAX_GENMASK]) != NULL) {
        printf("genmask: %s\n", sock_masktop(sa, sa->sa_len));
    }

    exit(0);
}
