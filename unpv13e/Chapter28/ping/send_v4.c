#include "ping.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>

#ifndef u_short
typedef unsigned short u_short;
#endif

// in_cksum.c
uint16_t in_cksum(uint16_t *addr, int len);

void send_v4(void)
{
    int          len;
    struct icmp *icmp;

    icmp = (struct icmp *) sendbuf;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = pid;
    icmp->icmp_seq = nsent++;
    memset(icmp->icmp_data, 0xa5, datalen); // fill with pattern
    gettimeofday((struct timeval *) icmp->icmp_data, NULL);

    len = 8 + datalen;  // checksum ICMP header and data
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = in_cksum((u_short *) icmp, len);

    sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen);
}
