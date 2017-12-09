#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAXLINE 4096 /* max text line length */

char *Sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
int udp_client(const char *host, const char *serv, struct sockaddr **saptr, socklen_t *lenp);

int main(int argc, char **argv)
{
    int              sockfd, n;
    char             recvline[MAXLINE + 1];
    socklen_t        salen;
    struct sockaddr *sa;

    if (argc != 3) {
        fprintf(stderr, "usage: daytimeudpcli1 <hostname/IPaddress> <service/port#>\n");
        exit(EXIT_FAILURE);
    }

    sockfd = udp_client(argv[1], argv[2], &sa, &salen);

    printf("sending to %s\n", Sock_ntop_host(sa, salen));

    if (sendto(sockfd, "", 1, 0, sa, salen) == -1) {
        fprintf(stderr, "sendto error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) == -1) {
        fprintf(stderr, "recvfrom error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    recvline[n] = '\0'; // null terminal
    fputs(recvline, stdout);
    exit(EXIT_SUCCESS);
}

// $ gcc daytimeudpcli1.c udp_client.c ../lib/sock_ntop_host.c -o daytimeudpcli1
