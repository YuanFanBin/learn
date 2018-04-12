#include "../lib/error.h"
#include "../lib/unpifi.h"
#include <net/if_arp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

// ../lib/sock_ntop.c
char * sock_ntop(const struct sockaddr *sa, socklen_t salen);

int main(int argc, char **argv)
{
    int                 sockfd;
    struct ifi_info    *ifi;
    unsigned char      *ptr;
    struct arpreq       arpreq;
    struct sockaddr_in *sin;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        err_sys("socket failed");
    }
    for (ifi = get_ifi_info(AF_INET, 0); ifi != NULL; ifi = ifi->ifi_next) {
        printf("%s: ", sock_ntop(ifi->ifi_addr, sizeof(struct sockaddr_in)));
        sin = (struct sockaddr_in *) &arpreq.arp_pa;
        memcpy(sin, ifi->ifi_addr, sizeof(struct sockaddr_in));
        if (ioctl(sockfd, SIOCGARP, &arpreq) < 0) {
            err_ret("ioctl SIOCGARP");
            continue;
        }
        ptr = &arpreq.arp_ha.sa_data[0];
        printf("%x:%x:%x:%x:%x:%x\n", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
    }
    exit(0);
}

// $ gcc prmac.c ../lib/error.c ../lib/sock_ntop.c get_ifi_info.c -o prmac
// $ ./prmac
// 127.0.0.1: ioctl SIOCGARP: No such device
