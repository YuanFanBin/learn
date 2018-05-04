#include "../lib/error.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int       sockfd;
    int       rcvbuf_len;
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        err_sys("socket error");
    }
    len = sizeof(int);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_len, &len) == -1) {
        err_sys("getsockopt error");
    }
    printf("SO_RCVBUF: %d\n", rcvbuf_len);
    close(sockfd);
}

// $ gcc so_rcvbuf.c ../lib/error.c ../lib/error.h
