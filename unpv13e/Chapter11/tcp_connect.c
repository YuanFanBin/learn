#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int tcp_connect(const char *host, const char *serv)
{
    int              sockfd, n;
    struct addrinfo  hints;
    struct addrinfo *res;
    struct addrinfo *ressave;

    // memset(&hints, 0, sizeof(struct addrinfo)); // or this
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        fprintf(stderr, "tcp_connect error for %s, %s: %s\n", host, serv, gai_strerror(n));
        exit(EXIT_FAILURE);
    }
    ressave = res;
    do {
        if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
            continue; // ignore this one
        }
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
            break; // success
        }
        close(sockfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        fprintf(stderr, "tcp_connect error for %s, %s\n", host, serv);
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(ressave);
    return(sockfd);
}
