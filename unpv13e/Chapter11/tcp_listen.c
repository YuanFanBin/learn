#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define LISTENQ 1024

int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
    int              listenfd, n;
    const int        on = 1;
    struct addrinfo  hints;
    struct addrinfo *res;
    struct addrinfo *ressave;

    // memset(&hints, 0, sizeof(struct addrinfo)); // or this
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        fprintf(stderr, "tcp_listen error for %s, %s: %s", host, serv, gai_strerror(n));
        exit(EXIT_FAILURE);
    }
    ressave = res;
    do {
        if ((listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
            continue; // error, try next one
        }
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
            fprintf(stderr, "setsockopt error, %s", strerror(errno));
            freeaddrinfo(ressave);
            exit(EXIT_FAILURE);
        }
        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0) {
            break;
        }
        close(listenfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        fprintf(stderr, "tcp_listen error for %s, %s", host, serv);
        exit(EXIT_FAILURE);
    }
    if (listen(listenfd, LISTENQ) == -1) {
        freeaddrinfo(ressave);
        exit(EXIT_FAILURE);
    }
    if (addrlenp) {
        *addrlenp = res->ai_addrlen; // return size of protocol address
    }
    freeaddrinfo(ressave);

    return(listenfd);
}
