#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/error.h"

#define UNIXSTR_PATH    "/tmp/unix.str"     /* Unix domain stream */

// Chapter08/dg_cli.c
void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen);

int main(int argc, char **argv)
{
    int     sockfd;
    struct sockaddr_un cliaddr, servaddr;

    if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
        err_sys("socket error");
    }
    // 在TCP，UDP协议中connect调用会给本套接字捆绑一个路径名
    // 但Unix域套接字不会主动捆绑一个路径名，我们需要手动绑定
    // 一个路径，否则Unix域套接字无法回射应答
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_LOCAL;
    strcpy(cliaddr.sun_path, tmpnam(NULL));
    if (bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0) {
        err_sys("bind error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    exit(0);
}

// $ gcc unixdgcli01.c ../lib/error.c ../Chapter08/dg_cli.c -o unixdgcli01
