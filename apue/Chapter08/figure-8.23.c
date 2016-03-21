#include "apue.h"
#include <sys/wait.h>

/* gcc apue.h apue_err.c figure-8.5.c figure-8.22.c figure-8.23.c */
int
main(void)
{
    int status;

    if ((status = system("date")) < 0) {    /* figure-8.22.c */
        err_sys("system() error");
    }
    pr_exit(status);    /* figure-8.5.c */

    if ((status = system("nosuchcommand")) < 0) {
        err_sys("system() error");
    }
    pr_exit(status);

    if ((status = system("who; exit 44")) < 0) {
        err_sys("system() error");
    }
    pr_exit(status);

    exit(0);
}

