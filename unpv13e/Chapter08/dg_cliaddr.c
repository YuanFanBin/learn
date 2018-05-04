#include "../lib/error.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * Sock_ntop(const struct sockaddr *sa, socklen_t salen);

#define MAXLINE 4096 /* max text line length */

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen) {
    int                 n;
    char                sendline[MAXLINE];
    char                recvline[MAXLINE + 1];
    char                srv_ip[16];
    socklen_t           len;
    struct sockaddr    *preply_addr;
    struct sockaddr_in *sa;

    printf("[dg_cliaddr.c]: sendto, recvfrom.\n");
    printf("[若服务器为多宿，则可能失败]\n");
    fflush(stdout);

    preply_addr = (struct sockaddr *) malloc(servlen);

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) == -1) {
            err_sys("sendto error");
        }
        len = servlen;
        if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len)) == -1) {
            err_sys("recvfrom error");
        }
        sa = (struct sockaddr_in *) preply_addr;
        if (inet_ntop(AF_INET, &sa->sin_addr, srv_ip, 16) == NULL) {
            err_sys("inet_ntop error");
        }
        printf("[server ip] %s\n", srv_ip);
        if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0) {
            printf("reply from %s (ignored)\n", Sock_ntop(preply_addr, len));
            continue;
        }
        recvline[n] = 0; /* null terminate */
        fputs(recvline, stdout);
    }
    free(preply_addr);
}
