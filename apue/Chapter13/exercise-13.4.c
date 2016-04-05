#include "apue.h"

/* gcc apue.h apue_err.c figure-13.1.c exercise-13.4.c */
int
main(void)
{
    FILE *fp;
    char *p;

    daemonize("getlog");    /* figure-13.1.c */
    p = getlogin();
    fp = fopen("/tmp/getlog.out", "w");
    if (fp != NULL) {
        if (p == NULL)
            fprintf(fp, "no login name\n");
        else
            fprintf(fp, "login name: %s\n", p);
    }
    exit(0);
}
