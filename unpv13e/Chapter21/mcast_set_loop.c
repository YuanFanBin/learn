#include "mcast.h"

int mcast_set_loop(int sockfd, int onoff)
{
    u_char flag;
#ifdef IPV6
    u_int flag;
#endif
    switch (sockfd_to_family(sockfd)) {
        case AF_INET:
            flag = onoff;
            return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, sizeof(flag)));
#ifdef IPV6
        case AF_INET6:
            flag = onoff;
            return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &flag, sizeof(flag)));
#endif
        default:
            errno = EAFNOSUPPORT;
            return(-1);
    }
}
