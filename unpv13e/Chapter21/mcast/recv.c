#include "../../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXLINE     4096    // max text line length

// ../lib/scok_ntop.c
char *sock_ntop(const struct sockaddr *sa, socklen_t salen);

void recv_all(int recvfd, socklen_t salen)
{
    int              n;
    char             line[MAXLINE + 1];
    socklen_t        len;
    struct sockaddr *safrom;

    safrom = (struct sockaddr *) malloc(salen);
    for ( ; ; ) {
        len = salen;
        n = recvfrom(recvfd, line, MAXLINE, 0, safrom, &len);
        line[n] = 0;
        printf("from %s: %s", sock_ntop(safrom, len), line);
    }
}
