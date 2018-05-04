#include "../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#ifndef     SUN_LEN
# define    SUN_LEN(su) \
    (sizeof(*(su)) - sizeof((su)->sun_path) + strlen((su)->sun_path))
#endif

int main(int argc, char **argv)
{
    int                 sockfd;
    socklen_t           len;
    struct sockaddr_un  addr1, addr2;

    if (argc != 2) {
        err_quit("usage: unixbind <pathname>");
    }

    if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
        err_sys("socket error");
    }

    unlink(argv[1]);    // OK if this fails

    bzero(&addr1, sizeof(addr1));
    addr1.sun_family = AF_LOCAL;
    strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path) - 1);
    if (bind(sockfd, (struct sockaddr *) &addr1, SUN_LEN(&addr1)) == -1) {
        err_sys("bind error");
    }

    len = sizeof(addr2);
    if (getsockname(sockfd, (struct sockaddr *) &addr2, &len) == -1) {
        err_sys("getsockname error");
    }
    printf("bound name = %s returned len = %d\n", addr2.sun_path, len);

    exit(0);
}

// $ gcc unixbind.c ../lib/error.c -o unixbind
// $ ./unixbind /tmp/moose
// bound name = /tmp/moose returned len = 13
