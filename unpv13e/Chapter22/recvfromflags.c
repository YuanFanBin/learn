#include "../lib/error.h"
#include "pktinfo.h"
#include <string.h>
#include <strings.h>
#include <sys/param.h>  // ALIGN macro fro CMSG_NXTHDR() macro
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>    // struct iovec{}

// recvfrom_flags 返回标志和目的地址
ssize_t recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp, struct sockaddr *sa, socklen_t *salenptr, struct unp_in_pktinfo *pktp)
{
    struct msghdr   msg;
    struct iovec    iov[1];
    ssize_t         n;

#ifdef HAVE_MSGHDR_MSG_CONTROL
    struct cmsghdr  *cmptr;
    union {
        struct cmsghdr  cm;
        char            control[CMSG_SPACE(sizeof(struct in_addr)) + CMSG_SPACE(sizeof(struct unp_in_pktinfo))];
    } control_un;
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
    msg.msg_flags = 0;
#else
    bzero(&msg, sizeof(msg));       // make certain msg_accrightslen = 0
#endif

    msg.msg_name = sa;
    msg.msg_namelen = *salenptr;
    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    if ((n = recvmsg(fd, &msg, *flagsp)) < 0) {
        return(n);
    }
    *salenptr = msg.msg_namelen;    // pass back results
    if (pktp) {
        bzero(pktp, sizeof(struct unp_in_pktinfo));     // 0.0.0.0, i/f = 0
    }

#ifndef HAVE_MSGHDR_MSG_CONTROL
    *flagsp = 0;                    // pass back result
    return(n);
#else
    *flagsp = msg.msg_flags;        // pass back results
    if (msg.msg_controllen < sizeof(struct cmsghdr) || (msg.msg_flags & MSG_CTRUNC) || pktp == NULL) {
        return(n);
    }
    for (cmptr = CMSG_FIRSTHDR(&msg); cmptr != NULL; cmptr = CMSG_NXTHDR(&msg, cmptr)) {
#ifdef IP_RECVDSTADDR // 返回目的IP地址
        if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_RECVDSTADDR) {
            memcpy(&pktp->ipi_addr, CMSG_DATA(cmptr), sizeof(struct in_addr));
            continue;
        }
#endif
#ifdef IP_RECVIF // 返回接收接口索引
        if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_RECVIF) {
            struct sockaddr_dl *sdl;
            sdl = (struct sockaddr_dl *) CMSG_DATA(cmptr);
            pktp->ipi_ifindex = sdl->sdl_index;
            continue;
        }
#endif
        err_quit("unknown ancillary data, len = %d, level = %d, type = %d", cmptr->cmsg_len, cmptr->cmsg_level, cmptr->cmsg_type);
    }
    return(n);
#endif      // HAVE_MSGHDR_MSG_CONTROL
}
