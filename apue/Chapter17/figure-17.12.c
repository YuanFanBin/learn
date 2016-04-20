#include "apue.h"

/*
 * Used when we had planned to send an fd using send_fd(),
 * but encountered an error instead. We send the error back 
 * using the send_fd()/recv_fd() protocol.
 */
/* figure-14.24.c */
int
send_err(int fd, int errcode, const char *msg)
{
    int     n;

    if ((n = strlen(msg)) > 0)
        /* figure-14.24.c */
        if (writen(fd, msg, n) != n)    /* send the error msg */
            return (-1);

    if (errcode >= 0)
        errcode = -1;   /* must be nagative */

    if (send_fd(fd, errcode) < 0)
        return(-1);

    return(0);
}
