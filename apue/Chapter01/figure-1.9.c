#include "apue.h"

/* gcc apue.h figure-1.9.c */
int
main(void)
{
    printf("uid = %d, gid = %d\n", getuid(), getgid());
    exit(0);
}
