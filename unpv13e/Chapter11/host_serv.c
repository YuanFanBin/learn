#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype)
{
    int              n;
    struct addrinfo  hints;
    struct addrinfo *res;

    // memset(&hints, 0, sizeof(struct addrinfo)); // or this
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_CANONNAME; // always return canonical name
    hints.ai_family = family;      // AF_UNSPEC, AF_INET, AF_INET6, etc.
    hints.ai_socktype = socktype;  // 0, SOCK_STREAM, SOCK_DGRAM, etc.
    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        return(NULL);
    }
    return(res);
}
