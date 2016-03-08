#include "apue.h"
#include <errno.h>

/* gcc apue.h apue_err.c figure-1.8.c */
int
main(int argc, char *argv[])
{
    fprintf(stderr, "EACCESS: %s\n", strerror(EACCES));
    errno = ENOENT;
    perror(argv[0]);
    exit(0);
}
