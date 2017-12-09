#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */

int tcp_connect(const char *host, const char *serv);

int main(int argc, char **argv)
{
    int                     sockfd, n;
    char                    recvline[MAXLINE + 1];
    socklen_t               len;
    struct sockaddr_storage ss;

    if (argc != 3) {
        fprintf(stderr, "usage: daytimetcpcli <hostname/IPaddress> <service/port#>\n");
        exit(EXIT_FAILURE);
    }
    sockfd = tcp_connect(argv[1], argv[2]);
    len = sizeof(ss);
    if (getpeername(sockfd, (struct sockaddr *) &ss, &len) == -1) {
        fprintf(stderr, "getpeername error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0; // null terminate
        fputs(recvline, stdout);
    }
    exit(EXIT_SUCCESS);
}

// $ gcc daytimetcpcli.c tcp_connect.c -o daytimetcpcli
