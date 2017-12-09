#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

int udp_client(const char *host, const char *serv, struct sockaddr **saptr, socklen_t *lenp)
{
    int              sockfd, n;
    struct addrinfo  hints;
    struct addrinfo *res;
    struct addrinfo *ressave;

    // memset(&hints, 0, sizeof(struct addrinfo)); // or this
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        fprintf(stderr, "udp_client error for %s, %s: %s", host, serv, gai_strerror(n));
        exit(EXIT_FAILURE);
    }
    ressave = res;

    do {
        if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) > 0) {
            break; // success
        }
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) { // errno set from final socket()
        fprintf(stderr, "udp_client error for %s, %s", host, serv);
        exit(EXIT_FAILURE);
    }
    *saptr = (struct sockaddr *) malloc(res->ai_addrlen);
    memcpy(*saptr, res->ai_addr, res->ai_addrlen);
    *lenp = res->ai_addrlen;

    freeaddrinfo(ressave);
    return(sockfd);
}
