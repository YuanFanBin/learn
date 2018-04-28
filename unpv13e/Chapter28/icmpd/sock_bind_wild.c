#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

int sock_bind_wild(int sockfd, int family)
{
    socklen_t	        len;
    struct sockaddr_in	sin;
#ifdef	IPV6
    struct sockaddr_in6	sin6;
#endif

    switch (family) {
        case AF_INET:
            bzero(&sin, sizeof(sin));
            sin.sin_family = AF_INET;
            sin.sin_addr.s_addr = htonl(INADDR_ANY);
            sin.sin_port = htons(0);	/* bind ephemeral port */

            if (bind(sockfd, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
                return(-1);
            }
            len = sizeof(sin);
            if (getsockname(sockfd, (struct sockaddr *) &sin, &len) < 0) {
                return(-1);
            }
            return(sin.sin_port);

#ifdef	IPV6
        case AF_INET6:
            bzero(&sin6, sizeof(sin6));
            sin6.sin6_family = AF_INET6;
            sin6.sin6_addr = in6addr_any;
            sin6.sin6_port = htons(0);	/* bind ephemeral port */

            if (bind(sockfd, (struct sockaddr *) &sin6, sizeof(sin6)) < 0)
                return(-1);
            len = sizeof(sin6);
            if (getsockname(sockfd, (struct sockaddr *) &sin6, &len) < 0)
                return(-1);
            return(sin6.sin6_port);
#endif
    }
    return(-1);
}
