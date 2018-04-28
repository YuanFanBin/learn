#include "ping.h"
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

void readloop(void)
{
    int             size;
    char            recvbuf[BUFSIZE];
    char            controlbuf[BUFSIZE];
    struct msghdr   msg;
    struct iovec    iov;
    ssize_t         n;
    struct timeval  tval;

    if ((sockfd = socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto)) < 0) {
        err_sys("socket error");
    }
    setuid(getuid()); // don't need special permissions any more
    if (pr->finit) {
        (*pr->finit)();
    }

    size = 60 * 1024;   // OK if setsockopt fails
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    sig_alrm(SIGALRM);  // send first packet

    iov.iov_base = recvbuf;
    iov.iov_len = sizeof(recvbuf);
    msg.msg_name = pr->sarecv;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = controlbuf;
    for ( ; ;) {
        msg.msg_namelen = pr->salen;
        msg.msg_controllen = sizeof(controlbuf);
        n = recvmsg(sockfd, &msg, 0);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            err_sys("recvmsg error");
        }
        gettimeofday(&tval, NULL);
        (*pr->fproc)(recvbuf, n, &msg, &tval);
    }
}