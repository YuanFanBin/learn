#include "../lib/error.h"
#include "unpifi.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ../lib/sock_ntop_host.c
char * sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

int main(int argc, char **argv)
{
    struct ifi_info *ifi, *ifihead;
    struct sockaddr *sa;
    u_char          *ptr;
    int              i, doaliases;
    int              family = AF_INET;

    if (argc != 3) {
        err_quit("usage: prifinfo <inet4|inet6> <doaliases>");
    }

    if (strcmp(argv[1], "inet4") == 0) {
        family = AF_INET;
    } else if (strcmp(argv[1], "inet6") == 0) {
        family = AF_INET6;
    } else {
        err_quit("invalid <address-family>");
    }
    doaliases = atoi(argv[2]);

    for (ifihead = ifi = get_ifi_info(family, doaliases); ifi != NULL; ifi = ifi->ifi_next) {
        printf("%s: ", ifi->ifi_name);
        if (ifi->ifi_index != 0) {
            printf("(%d) ", ifi->ifi_index);
        }
        printf("<");
        if (ifi->ifi_flags & IFF_UP)            printf("UP ");
        if (ifi->ifi_flags & IFF_BROADCAST)     printf("BCAST ");
        if (ifi->ifi_flags & IFF_MULTICAST)     printf("MCAST ");
        if (ifi->ifi_flags & IFF_LOOPBACK)      printf("LOOP ");
        if (ifi->ifi_flags & IFF_POINTOPOINT)   printf("P2P ");
        printf(">\n");

        if ((i = ifi->ifi_hlen) > 0) {
            ptr = ifi->ifi_haddr;
            do {
                printf("%s%x", (i == ifi->ifi_hlen) ? "  " : ":", *ptr++);
            } while (--i > 0);
        }
        if (ifi->ifi_mtu != 0) {
            printf("  MTU: %d\n", ifi->ifi_mtu);
        }
        if ((sa = ifi->ifi_addr) != NULL) {
            printf("  IP addr: %s\n", sock_ntop_host(sa, sizeof(*sa)));
        }
        if ((sa = ifi->ifi_brdaddr) != NULL) {
            printf("  broadcast addr: %s\n", sock_ntop_host(sa, sizeof(*sa)));
        }
        if ((sa = ifi->ifi_dstaddr) != NULL) {
            printf("  destination addr: %s\n", sock_ntop_host(sa, sizeof(*sa)));
        }
    }
    free_ifi_info(ifihead);
    exit(0);
}

// $ gcc prifinfo.c ../lib/error.c ../lib/sock_ntop_host.c get_ifi_info.c -o prifinfo
// $ ./prifinfo inet4 0
