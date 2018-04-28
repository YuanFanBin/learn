#include "icmpd.h"

// ../Chapter15/read_fd.c
ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);
// sock_bind_wild.c
int sock_bind_wild(int sockfd, int family);
// sock_get_port.c
int sock_get_port(const struct sockaddr *sa, socklen_t salen);

int readable_conn(int i)
{
    int                     unixfd, recvfd;
    char                    c;
    ssize_t                 n;
    socklen_t               len;
    struct sockaddr_storage ss;

    unixfd = client[i].connfd;
    recvfd = -1;
    if ((n = read_fd(unixfd, &c, 1, &recvfd)) == 0) {
        err_msg("client %d terminated, recvfd = %d", i, recvfd);
        goto clientdone;    // client probably terminated
    }

    // data from client; should be descriptor
    if (recvfd < 0) {
        err_msg("read_fd did not return descriptor");
        goto clienterr;
    }

    len = sizeof(ss);
    if (getsockname(recvfd, (struct sockaddr *) &ss, &len) < 0) {
        err_ret("getsockname error");
        goto clienterr;
    }

    client[i].family = ss.ss_family;
    if ((client[i].lport = sock_get_port((struct sockaddr *) &ss, len)) == 0) {
        client[i].lport = sock_bind_wild(recvfd, client[i].family);
        if (client[i].lport <= 0) {
            err_ret("error binding ephemeral port");
            goto clienterr;
        }
    }
    write(unixfd, "1", 1);  // tell client all OK
    close(recvfd);  // all done with client's UDP socket
    return(--nready);

clienterr:
    write(unixfd, "0", 1);  // tell client error occurred
clientdone:
    close(unixfd);
    if (recvfd >= 0) {
        close(recvfd);
    }
    FD_CLR(unixfd, &allset);
    client[i].connfd = -1;
    return(--nready);
}
