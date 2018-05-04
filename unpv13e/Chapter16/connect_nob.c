#include "../lib/error.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

int connect_nonb(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    int             flags, n, error;
    socklen_t       len;
    fd_set          rset, wset;
    struct timeval  tval;

    if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        err_sys("fcntl error");
    }

    error = 0;
    if ((n = connect(sockfd, saptr, salen)) < 0) {
        if (errno != EINPROGRESS) { // 连接建立已经启动但尚未完成
            return(-1);
        }
    }

    // Do whatever we want while the connect is taking palce.
    if (n == 0) {
        goto done;  // connect completed immediately，无错误
    }
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tval.tv_sec = nsec;
    tval.tv_usec = 0;
    if ((n = select(sockfd + 1, &rset, &wset, NULL, nsec ? &tval : NULL)) < 0) {
        err_sys("select error");
    } else if (n == 0) { // 超时且没有描述符准备好
        close(sockfd); // 关闭socket，三次握手没必要进行下去
        errno = ETIMEDOUT;
        return(-1);
    }
    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
        len = sizeof(error);
        // connect 出错，sockfd描述符可读＆可写，通过getsockopt获取错误信息，并重置sockfd标识位
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
            return(-1);
        }
    } else {
        err_quit("select error: sockfd not set");
    }

done:
    if (fcntl(sockfd, F_SETFL, flags) < 0) { // restore file status flags
        err_sys("fcntl error");
    }
    if (error) {
        close(sockfd); // just in case
        errno = error;
        return(-1);
    }
    return(0);
}
