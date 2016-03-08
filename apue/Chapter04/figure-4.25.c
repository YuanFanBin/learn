#include "apue.h"
#ifdef SOLARIS
#define _BSD_SOURCE
#include <sys/mkdev.h>
#endif
#include <sys/sysmacros.h>

/* gcc apue.h apue_err.c figure-4.25.c */
int
main(int argc, char *argv[])
{
    int         i;
    struct stat buf;
#ifdef _BSD_SOURCE
    printf("_BSD_SOURCE defined\n");
#endif

    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (stat(argv[i], &buf) < 0) {
            err_ret("stat error");
            continue;
        }
        printf("dev = %d/%d", major(buf.st_dev), minor(buf.st_dev));
        if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
            printf(" (%s) rdev = %d/%d", 
                   (S_ISCHR(buf.st_mode)) ? "character" : "block",
                   major(buf.st_rdev), minor(buf.st_rdev));
        }
        printf("\n");
    }
    exit(0);
}
