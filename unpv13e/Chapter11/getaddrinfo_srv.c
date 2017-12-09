#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 500

int main(int argc, char *argv[])
{
    struct addrinfo         hints;
    struct addrinfo        *result, *rp;
    int                     sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t               peer_addr_len;
    ssize_t                 nread;
    char                    buf[BUF_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     // allow IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;  // datagram socket
    hints.ai_flags = AI_PASSIVE;     // for wildcard IP address
    hints.ai_protocol = 0;           // any protocol
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if ((s = getaddrinfo(NULL, argv[1], &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if ((sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1) {
            continue;
        }
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break; // success
        }
        close(sfd);
    }
    if (rp == NULL) { // no address succeeded
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result); // no longer needed

    // read datagrams and echo them back to sender
    for (;;) {
        peer_addr_len = sizeof(struct sockaddr_storage);
        if ((nread = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &peer_addr, &peer_addr_len)) == -1) {
            continue; // ignore failed request
        }
        char host[NI_MAXHOST], service[NI_MAXSERV];
        if ((s = getnameinfo((struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV)) == 0) {
            printf("received %zd bytes from %s:%s\n", nread, host, service);
        } else {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
        }
        if (sendto(sfd, buf, nread, 0, (struct sockaddr *) &peer_addr, peer_addr_len) != nread) {
            fprintf(stderr, "Error sending response\n");
        }
    }
    exit(EXIT_FAILURE);
}
