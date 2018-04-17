#include "mcast.h"

int family_to_level(int family)
{
    switch (family) {
        case AF_INET:
            return IPPROTO_IP;
#ifdef  IPV6
        case AF_INET6:
            return IPPROTO_IPV6;
#endif
        default:
            return -1;
    }
}

#define MAXSOCKADDR 128     // max socket address structure size

int sockfd_to_family(int sockfd)
{
    union {
        struct sockaddr   sa;
        char              data[MAXSOCKADDR];
    } un;
    socklen_t   len;

    len = MAXSOCKADDR;
    if (getsockname(sockfd, (struct sockaddr *) un.data, &len) < 0) {
        return(-1);
    }
    return(un.sa.sa_family);
}
