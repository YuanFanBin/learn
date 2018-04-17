#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifdef IPV6
    const struct in6_addr in6addr_any;
#endif

void sock_set_addr(struct sockaddr *sa, socklen_t salen, const void *addr)
{
    struct sockaddr_in *sin;
#ifdef IPV6
    struct sockaddr_in6 *sin6;
#endif
    switch (sa->sa_family) {
        case AF_INET:
            sin = (struct sockaddr_in *) sa;
            memcpy(&sin->sin_addr, addr, sizeof(struct in_addr));
            return;

#ifdef IPV6
        case AF_INET6:
            sin6 = (struct sockaddr_in6 *) sa;
            memcpy(&sin6->sin6_addr, addr, sizeof(struct in6_addr));
            return;
#endif
    }
}

void sock_set_wild(struct sockaddr *sa, socklen_t salen)
{
    const void *wildptr;
    static struct in_addr in4addr_any;

    switch (sa->sa_family) {
        case AF_INET:
            in4addr_any.s_addr = htonl(INADDR_ANY);
            wildptr = &in4addr_any;
            break;

#ifdef IPV6
        case AF_INET6:
            wildptr = &in6addr_any;
            break;
#endif

        default:
            return;
    }
    sock_set_addr(sa, salen, wildptr);
}
