#include "../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIXSTR_PATH    "/tmp/unix.str"     /* Unix domain stream */

// Chapter05/str_cli.c
void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_un  servaddr;

    if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        err_sys("connect error");
    }
    str_cli(stdin, sockfd);
    exit(0);
}

// $ gcc unixstrcli01.c ../lib/error.c ../Chapter03/readline.c ../Chapter05/str_cli.c -o unixstrcli01
