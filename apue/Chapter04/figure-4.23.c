#include "apue.h"

/* gcc apue.h apue_err.c figure-4.23 */
int
main(void)
{
    if (chdir("/tmp") < 0)
        err_sys("chdir failed");
    printf("chdir to /tmp succeeded\n");
    exit(0);
}
