#include "../../lib/error.h"
#include "../mcast.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void recv_all(int, socklen_t);
void send_all(int, struct sockaddr *, socklen_t);

// ../Chapter11/udp_client.c
int udp_client(const char *host, const char *serv, struct sockaddr **saptr, socklen_t *lenp);

int main(int argc, char **argv)
{
    int              sendfd, recvfd;
    const int        on = 1;
    socklen_t        salen;
    struct sockaddr *sasend, *sarecv;

    if (argc != 3) {
        err_quit("usage: sendrecv <IP-multicast-address> <port#>");
    }
    sendfd = udp_client(argv[1], argv[2], &sasend, &salen);
    recvfd = socket(sasend->sa_family, SOCK_DGRAM, 0);
    // SO_REUSEADDR 允许多个实例同时在单一主机上运行
    if (setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        err_sys("setsockopt error");
    }
    sarecv = (struct sockaddr *) malloc(salen);
    memcpy(sarecv, sasend, salen);
    if (bind(recvfd, sarecv, salen) < 0) {
        err_sys("bind error");
    }
    if (mcast_join(recvfd, sasend, salen, NULL, 0) < 0) {
        err_sys("mcast_join error");
    }
    if (mcast_set_loop(sendfd, 0) < 0) {
        err_sys("mcast_set_loop error");
    }
    if (fork() == 0) {
        recv_all(recvfd, salen);        // child -> receives
    }
    send_all(sendfd, sasend, salen);    // parent -> sends
    free(sarecv);
}
