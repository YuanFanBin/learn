#include "apue.h"
#include <sys/socket.h>
#include <sys/un.h>

/* gcc apue.h apue_err.c figure-17.5.c */
int
main(void)
{
    int                 fd, size;
    struct sockaddr_un  un;

    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, "foo.socket");
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        err_sys("socket failed");
    size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
    if (bind(fd, (struct sockaddr *)&un, size) < 0)
        err_sys("bind failed");
    printf("UNIX domain socket bound\n");
    unlink("foo.socket");
    exit(0);
}
