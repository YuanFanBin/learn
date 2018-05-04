#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int udp_server(const char *host, const char *serv, socklen_t *addrlenp)
{
    int              sockfd, n;
    struct addrinfo  hints;
    struct addrinfo *res;
    struct addrinfo *ressave;

    // memset(&hints, 0, sizeof(struct addrinfo)); // or this
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        fprintf(stderr, "udp_server error for %s, %s: %s", host, serv, gai_strerror(n));
        exit(EXIT_FAILURE);
    }
    ressave = res;

    do {
        if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
            continue;
        }
        if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
            break; // sucess
        }
        close(sockfd); // bind error - close and try next one
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) { // errno from final socket() or bind()
        fprintf(stderr, "udp_server error for %s, %s", host, serv);
        exit(EXIT_FAILURE);
    }
    if (addrlenp) {
        *addrlenp = res->ai_addrlen; // return size of protocol address
    }
    freeaddrinfo(ressave);
    return(sockfd);
}
