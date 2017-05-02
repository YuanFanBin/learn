#include <sys/socket.h>     /* getsockname, sockaddr_storage, sockaddr */

/* 返回套接字的地址族 */
int sockfd_to_family(int sockfd)
{
    struct sockaddr_storage ss;     /* 通用结构 */
    socklen_t               len;

    len = sizeof(ss);
    if (getsockname(sockfd, (struct sockaddr *) &ss, &len) < 0) {
        return(-1);
    }
    return(ss.ss_family);
}
