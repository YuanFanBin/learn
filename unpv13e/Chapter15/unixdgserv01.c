#include "../lib/error.h"
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIXSTR_PATH    "/tmp/unix.str"     /* Unix domain stream */

// Chapter08/dg_echo.c
void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen);

int main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_un  servaddr, cliaddr;

    if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
        err_sys("socket error");
    }
    unlink(UNIXSTR_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);
    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        err_sys("bind error");
    }
    dg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

// $ gcc unixdgserv01.c ../lib/error.c ../Chapter08/dg_echo.c -o unixdgserv01
