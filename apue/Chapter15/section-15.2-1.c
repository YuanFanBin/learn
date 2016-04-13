#include "apue.h"
#include <errno.h>

static int 
pipe_buf_max(int fd)
{
#ifdef _PC_PIPE_BUF
    long val;
    errno = 0;
    if ((val = fpathconf(fd, _PC_PIPE_BUF)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL) {
                fprintf(stdout, " (not supported _SC_PIPE_BUF)\n");
            } else {
                fprintf(stderr, " fpathconf error, fd = %d", fd);
                return(0);
            }
        } else {
            fprintf(stdout, " (no limit)\n");
        }
    } else {
        return(val);
    }
    return(0);
#else
    fprintf(stdout, " (not supported _SC_PIPE_BUF)\n");
    return(0);
#endif
}

/* gcc apue.h apue_err.c section-15.2-1.c */
int
main(int argc, char *argv[])
{
    int     fd[2];
    pid_t   pid;
    if (pipe(fd) < 0) {
        err_sys("pipe error");
    }
    printf("%d\n", pipe_buf_max(fd[1]));
    exit(0);
}
