#include "web.h"
#include <stdio.h>
#include <unistd.h>

// Chapter03/writen.c
ssize_t writen(int fd, const void *vptr, size_t n);

void write_get_cmd(struct file *fptr)
{
    int n;
    char line[MAXLINE];
    n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name);
    if (writen(fptr->f_fd, line, n) < 0) {
        err_sys("writen error");
    }
    printf("wrote %d bytes for %s\n", n, fptr->f_name);

    fptr->f_flags = F_READING; // clears F_CONNECTING

    FD_SET(fptr->f_fd, &rset); // will read server's reply
    if (fptr->f_fd > maxfd) {
        maxfd = fptr->f_fd;
    }
}
