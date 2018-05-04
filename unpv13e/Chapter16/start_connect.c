#include "web.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>

// Chapter11/host_serv.c
struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);

void start_connect(struct file *fptr)
{
    int fd, flags, n;
    struct addrinfo *ai;

    if ((ai = host_serv(fptr->f_host, SERV, 0, SOCK_STREAM)) < 0) {
        err_sys("host_serv error");
    }
    if ((fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
        err_sys("socket error");
    }
    fptr->f_fd = fd;
    printf("start_connect for %s, fd %d\n", fptr->f_name, fd);

    // set socket nonblocking
    if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        err_sys("fcntl error");
    }

    // initiate nonblocking connect to the server
    if ((n = connect(fd, ai->ai_addr, ai->ai_addrlen)) < 0) {
        if (errno != EINPROGRESS) {
            err_sys("nonblocking connect error");
        }
        fptr->f_flags = F_CONNECTING;
        FD_SET(fd, &rset); // select for reading and writing
        FD_SET(fd, &wset);
        if (fd > maxfd) {
            maxfd = fd;
        }
    } else if (n >= 0) { // connect is already done
        write_get_cmd(fptr); // write the GET command
    }
}
