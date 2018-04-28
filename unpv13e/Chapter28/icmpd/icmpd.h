#ifndef __icmpd_h
#define __icmpd_h

#include "../../lib/error.h"
#include "unpicmpd.h"
#include <arpa/inet.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#ifdef IPV6
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#endif

#define MAXLINE     4096        /* max text line length */
#define LISTENQ 1024            /* 2nd argument to listen() */
#define max(a, b)   ((a) > (b) ? (a) : (b))

struct client {
    int connfd; // Unix domain stream socket to client
    int family; // AF_INET or AF_INET6
    int lport;  // local port bound to client's UDP socket
    // network byte ordered
} client[FD_SETSIZE];

int                 fd4, fd6, listenfd, maxi, maxfd, nready;
fd_set              rset, allset;
struct sockaddr_un  cliaddr;

int readable_conn(int);
int readable_listen(void);
int readable_v4(void);
int readable_v6(void);

#endif // __icmpd_h
