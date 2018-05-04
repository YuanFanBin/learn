#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int udp_connect(const char *host, const char *serv)
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
        fprintf(stderr, "udp_connect error for %s, %s: %s", host, serv, gai_strerror(n));
        exit(EXIT_FAILURE);
    }
    ressave = res;

    do {
        if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
            continue;
        }
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
            break; // success
        }
        close(sockfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        fprintf(stderr, "udp_connect error for %s, %s", host, serv);
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(ressave);
    return(sockfd);
}
