#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int sock_get_port(const struct sockaddr *sa, socklen_t salen)
{
    struct sockaddr_in  *sin = (struct sockaddr_in *) sa;
#ifdef  IPV6
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;
#endif
    switch (sa->sa_family) {
        case AF_INET:
            return(sin->sin_port);
#ifdef  IPV6
        case AF_INET6:
            return(sin6->sin6_port);
#endif
    }
    return(-1);
}
