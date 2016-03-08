#include "apue.h"
#include <fcntl.h>

/* gcc apue.h apue_err.c figure-4.21.c */
int
main(int argc, char *argv[])
{
    int             i, fd;
    struct stat     statbuf;
    struct timespec times[2];
    for (i = 1; i < argc; ++i) {
        if (stat(argv[i], &statbuf) < 0) {  /* fetch current time */
            err_ret("%s: stat error", argv[i]);
            continue;
        }
        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {  /* truncate */
            err_ret("%s: open error", argv[i]);
            continue;
        }
        times[0] = statbuf.st_atime;
        times[1] = statbuf.st_mtime;
        if (futimens(fd, times) < 0)    /* reset times */
            err_ret("%s: futimens error", argv[i]);
        close(fd);
    }
}
