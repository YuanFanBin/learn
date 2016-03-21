#include "apue.h"

/* gcc apue.h figure-8.25.c -o printuids */
int
main(void)
{
    printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());
    exit(0);
}
