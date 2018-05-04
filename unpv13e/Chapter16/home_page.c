#include "web.h"
#include <stdio.h>
#include <unistd.h>

// Chapter11/tcp_connect.c
int tcp_connect(const char *host, const char *serv);
// Chapter03/writen.c
ssize_t writen(int fd, const void *vptr, size_t n);

void home_page(const char *host, const char *fname)
{
    int fd, n;
    char line[MAXLINE];

    fd = tcp_connect(host, SERV); // blocking connect
    n = snprintf(line, sizeof(line), GET_CMD, fname);
    if (writen(fd, line, n) < 0) {
        err_sys("write error");
    }
    for (;;) {
        if ((n = read(fd, line, MAXLINE)) == 0) {
            break; // server closed connection
        }
        printf("read %d bytes of home page\n", n);
    }
    printf("end-of-file on home page\n");
    close(fd);
}
