#include "apue.h"
#include <stdio.h>
#include <stdlib.h> /* EXIT_SUCCESS */
#include <unistd.h>
#include <errno.h>

/* gcc apue.h apue_err.c section-12.2.c */
int
main(int argc, char *argv[])
{
    long val;
    if ((val = sysconf(_SC_THREAD_THREADS_MAX)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL)
                fputs(" (not supported)\n", stdout);
            else
                err_sys("sysconf error");
        } else {
            fputs(" (no limit)\n", stdout); /* 没有确定的限制 */
        }
    } else {
        printf(" %ld\n", val);
    }
    exit(EXIT_SUCCESS);
}
