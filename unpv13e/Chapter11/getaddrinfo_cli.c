#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500

int main(int argc, char *argv[])
{
    struct addrinfo  hints;
    struct addrinfo *result, *rp;
    int              sfd, s, j;
    size_t           len;
    ssize_t          nread;
    char             buf[BUF_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     // allow IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;  // datagram socket
    hints.ai_flags = 0;
    hints.ai_protocol = 0;           // any protocol

    if ((s = getaddrinfo(argv[1], argv[2], &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if ((sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1) {
            continue;
        }
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break; // success
        }
        close(sfd);
    }

    freeaddrinfo(result); // no longer needed

    for (j = 3; j < argc; j++) {
        len = strlen(argv[j]) + 1;
        if (len + 1 > BUF_SIZE) {
            fprintf(stderr, "ignoring long message in argument %d\n", j);
            continue;
        }
        if (write(sfd, argv[j], len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }
        if ((nread = read(sfd, buf, BUF_SIZE)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("received %zd bytes: %s\n", nread, buf);
    }
    exit(EXIT_FAILURE);
}
