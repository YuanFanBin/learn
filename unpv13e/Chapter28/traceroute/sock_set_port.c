#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

void sock_set_port(struct sockaddr *sa, socklen_t salen, int port)
{
    struct sockaddr_in *sin = (struct sockaddr_in *) sa;
#ifdef  IPV6
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;
#endif
    switch (sa->sa_family) {
        case AF_INET:
            sin->sin_port = port;
            return;
#ifdef  IPV6
        case AF_INET6:
            sin6->sin6_port = port;
            return;
#endif
    }
}
