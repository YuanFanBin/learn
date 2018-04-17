#include "../mcast.h"
#include "../../lib/error.h"
#include <stdlib.h>

// 众所周知的域名和众所周知的端口
// sap.mcast.net(224.2.127.254)
#define SAP_NAME    "sap.mcast.net"     // default group name and port
#define SAP_PORT    "9875"

void loop(int, socklen_t);

// ../Chapter11/udp_client.c
int udp_client(const char *host, const char *serv, struct sockaddr **saptr, socklen_t *lenp);

int main(int argc, char **argv)
{
    int              sockfd;
    const int        on = 1;
    socklen_t        salen;
    struct sockaddr *sa;

    if (argc == 1) {
        sockfd = udp_client(SAP_NAME, SAP_PORT, &sa, &salen);
    } else if (argc == 4) {
        sockfd = udp_client(argv[1], argv[2], &sa, &salen);
    } else {
        err_quit("usage: mysdr <mcast-addr> <port#> <interface-name>");
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        err_sys("setsockopt error");
    }
    if (bind(sockfd, sa, salen) < 0) {
        err_sys("bind error");
    }
    if (mcast_join(sockfd, sa, salen, (argc == 4) ? argv[3] : NULL, 0) < 0) {
        err_sys("mcast_join error");
    }
    loop(sockfd, salen);    // receive and print
    exit(0);
}
