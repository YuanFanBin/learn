#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXLINE     4096        /* max text line length */

ssize_t dg_send_recv(int, const void*, size_t, void *, size_t, const struct sockaddr *, socklen_t);

// figure-8-8.png: (sendto, recvfrom) -> dg_send_recv
void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    ssize_t n;
    char    sendline[MAXLINE], recvline[MAXLINE + 1];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        n = dg_send_recv(sockfd, sendline, strlen(sendline), recvline, MAXLINE, pservaddr, servlen);
        recvline[n] = 0;
        fputs(recvline, stdout);
    }
}
