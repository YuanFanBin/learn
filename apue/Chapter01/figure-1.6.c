#include "apue.h"

/* gcc apue.h apue_err.c figure-1.6.c */
int
main(void)
{
    printf("hello world from process ID %ld\n", (long)getpid());
    exit(0);
}
