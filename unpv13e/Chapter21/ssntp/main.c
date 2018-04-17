#include "../../lib/error.h"
#include "../mcast.h"
#include "sntp.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// ../../lib/sock_ntop.c
char *sock_ntop(const struct sockaddr *sa, socklen_t salen);
// ../../Chapter11/udp_client.c
int udp_client(const char *host, const char *serv, struct sockaddr **saptr, socklen_t *lenp);
// sock_set_wild.c
void sock_set_wild(struct sockaddr *sa, socklen_t salen);

int main(int argc, char **argv)
{
    int              sockfd;
    char             buf[MAXLINE];
    ssize_t          n;
    socklen_t        salen, len;
    struct sockaddr *mcastsa, *wild, *from;
    struct timeval   now;

    if (argc != 2) {
        err_quit("usage: ssntp <IPaddress>");
    }
    sockfd = udp_client(argv[1], "ntp", &mcastsa, &salen);

    wild = (struct sockaddr *) malloc(salen);
    memcpy(wild, mcastsa, salen);       // copy family and port
    sock_set_wild(wild, salen);
    if (bind(sockfd, wild, salen) < 0) {    // bind wildcard
        err_sys("bind error");
    }

#ifdef MCAST
    // obtain interface list and process each one
    // ../../Chapter17/get_ifi_info.c
    for (ifi = get_ifi_info(mcastsa->sa_family, 1); ifi != NULL; ifi = ifi->ifi_next) {
        if (ifi->ifi_flags & IFF_MULTICAST) {
            mcast_join(sockfd, mcastsa, salen, ifi->ifi_name, 0);
            printf("joined %s on %s\n", sock_ntop(mcastsa, salen), ifi->ifi_name);
        }
    }
#endif

    from = (struct sockaddr *) malloc(salen);
    for ( ; ; ) {
        len = salen;
        n = recvfrom(sockfd, buf, sizeof(buf), 0, from, &len);
        gettimeofday(&now, NULL);
        sntp_proc(buf, n, &now);
    }
}
