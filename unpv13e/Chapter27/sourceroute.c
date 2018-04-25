#include "../lib/error.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef u_char
typedef unsigned char u_char;
#endif

static u_char *optr;        // popinter into options being formed
static u_char *lenptr;      // pointer to length byte in SRR option
static int     ocnt;        // count of # address

// ../Chapter11/host_serv.c
struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);

// 初始化一个源路径选项
u_char *inet_srcrt_init(int type)
{
    optr = (u_char *) malloc(44);   // NOP, code, len, ptr, up to 10 addresses
    bzero(optr, 44);                // guarantees EOLs at end
    ocnt = 0;
    *optr++ = IPOPT_NOP;            // NOP for alignment
    *optr++ = type ? IPOPT_SSRR : IPOPT_LSRR;
    lenptr = optr++;                // we fill in length later
    *optr++ = 4;                    // offset to first address
    return(optr - 4);
}

int inet_srcrt_add(char *hostptr)
{
    int                  len;
    struct addrinfo     *ai;
    struct sockaddr_in  *sin;

    if (ocnt > 9) {
        err_quit("too many source routes with: %s", hostptr);
    }
    ai = host_serv(hostptr, NULL, AF_INET, 0);
    sin = (struct sockaddr_in *) ai->ai_addr;
    memcpy(optr, &sin->sin_addr, sizeof(struct in_addr));
    freeaddrinfo(ai);

    optr += sizeof(struct in_addr);
    ocnt++;
    len = 3 + (ocnt * sizeof(struct in_addr));
    *lenptr = len;
    return(len + 1);        // sizeo for setsockopt()
}

// 接收源路径并显示该信息
void inet_srcrt_print(u_char *ptr, int len)
{
    u_char          c;
    char            str[INET_ADDRSTRLEN];
    struct in_addr  hop1;

    memcpy(&hop1, ptr, sizeof(struct in_addr));
    ptr += sizeof(struct in_addr);

    while ((c = *ptr++) == IPOPT_NOP) {} // skip any leading NOPs

    if (c == IPOPT_LSRR) {
        printf("received LSRR: ");
    } else if (c == IPOPT_SSRR) {
        printf("received SSRR: ");
    } else {
        printf("received option type %d\n", c);
        return;
    }
    printf("%s ", inet_ntop(AF_INET, &hop1, str, sizeof(str)));

    len = *ptr++ - sizeof(struct in_addr);     // subtract dest IP addr
    ptr++;
    while (len > 0) {
        printf("%s ", inet_ntop(AF_INET, ptr, str, sizeof(str)));
        ptr += sizeof(struct in_addr);
        len -= sizeof(struct in_addr);
    }
    printf("\n");
}
