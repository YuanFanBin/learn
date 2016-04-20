#include "figure-17.26.h"
#include <syslog.h>

int     debug, oflag, client_size, log_to_stderr;
char    errmsg[MAXLINE];
char   *pathname;
Client *client = NULL;

/* gcc apue.h apue_err.c apue_log.c figure-17.26.h figure-13.1.c figure-17.29.c figure-17.8.c figure-17.9.c figure-17.27.c figure-17.31.c figure-17.12.c figure-14.24.c figure-17.13.c figure-17.23.c figure-17.24.c figure-17.28.c -o opend */
int
main(int argc, char *argv[])
{
    int     c;

    log_open("open.serv", LOG_PID, LOG_USER);

    opterr = 0;     /* don't wait getopt() writing to stderr */
    while ((c = getopt(argc, argv, "d")) != EOF) {
        switch (c) {
        case 'd':   /* debug */
            debug = log_to_stderr = 1;
            break;

        case '?':
            err_quit("unrecongnized option: -%c", optopt);
        }
    }

    if (debug == 0)
        daemonize("opend"); /* figure-13.1.c */

    loop(); /* never returns */
}
