#include "mcast.h"

int mcast_leave(int sockfd, const struct sockaddr *grp, socklen_t grplen)
{
    switch (grp->sa_family) {
        case AF_INET:
            struct ip_mreq        mreq;
            memcpy(&mreq.imr_multiaddr, &((struct sockaddr_in *) grp)->sin_addr, sizeof(struct in_addr));
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq)));

#ifdef    IPV6
        case AF_INET6:
            struct ipv6_mreq    mreq6;
            memcpy(&mreq6.ipv6mr_multiaddr, &((struct sockaddr_in6 *) grp)->sin6_addr, sizeof(struct in6_addr));
            mreq6.ipv6mr_interface = 0;
            return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, &mreq6, sizeof(mreq6)));
#endif

        default:
            errno = EPROTONOSUPPORT;
            return(-1);
    }
}
