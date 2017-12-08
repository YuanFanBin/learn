#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

#define NDG         2000 /* datagrams to send */
#define DGLEN       1400 /* length of each datagram */

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen) {
    int  i;
    char sendline[DGLEN];

    printf("写固定数目的数据报到服务器\n");

    for (i = 0; i < NDG; i++) {
        if (sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) == -1) {
            err_sys("sendto error");
        }
    }
}
