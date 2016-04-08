#include "apue.h"
#include <fcntl.h>

/* gcc apue.h apue_err.c figure-3.12.c exercise-14.7.c */
int
main(void)
{
    int     i, n;
    int     fd[2];


    if (pipe(fd) < 0)
        err_sys("pipe error");
    set_fl(fd[1], O_NONBLOCK); /* figure-3.12.c */

    /* write 1 byte at a time until pipe is full */
    for (n = 0; ; n++) {
        if ((i = write(fd[1], "a", 1)) != 1) {
            printf("write ret %d, ", i);
            break;
        }
    }
    printf("pipe capacity = %d\n", n);
    exit(0);
}
