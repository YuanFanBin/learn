#include "apue.h"
#include <fcntl.h>

#define FIFO    "temp.fifo"

/* gcc apue.h apue_err.c figure-3.12.c exercise-15.10.c */
int
main(void)
{
    int     fdread, fdwrite;

    unlink(FIFO);
    if (mkfifo(FIFO, FILE_MODE) < 0)
        err_sys("mkfifo error");
    if ((fdread = open(FIFO, O_RDONLY | O_NONBLOCK)) < 0)
        err_sys("open error for reading");
    if ((fdwrite = open(FIFO, O_WRONLY)) < 0)
        err_sys("open error for waiting");
    clr_fl(fdread, O_NONBLOCK); /* figure-3.12.c */
    exit(0);
}
