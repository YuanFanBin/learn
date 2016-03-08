#include "apue.h"
#include <fcntl.h>

/* gcc apue.h apue_err.c figure-4.16.c */
int
main(void)
{
    if (open("tempfile", O_RDWR) < 0)
        err_sys("open error");
    if (unlink("tempfile") < 0)
        err_sys("unlink error");
    printf("file unlinked.\n");
    sleep(30);
    printf("done\n");
    exit(0);
}
