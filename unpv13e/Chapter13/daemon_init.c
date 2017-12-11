#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXFD 64

extern int daemon_proc; // defined in error.c

int daemon_init(const char *pname, int facility)
{
    int i;
    pid_t pid;

    if ((pid = fork()) < 0) {
        return(-1);
    } else if (pid) { // parent
        _exit(0);
    }
    // child 1
    // #1 子进程脱离父进程，有自己的进程ID，但继承了父进程的进程组ID

    // #2 创建新会话，当前进程变为新会话的会话头进程以及新进程组的进程组头，不再有控制终端。
    if (setsid() < 0) { // become session leader
        return(-1);
    }
    signal(SIGHUP, SIG_IGN);
    if ((pid = fork()) < 0) {
        return(-1);
    } else if (pid) {
        _exit(0); // child 1 terminates
    }
    // child 2
    daemon_proc = 1;
    chdir("/"); // change working directory

    // close off file descriptors
    for (i = 0; i < MAXFD; i++) {
        close(i);
    }
    // redirect stdin, stdout and stderr to /dev/null
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    openlog(pname, LOG_PID, facility);
    return(0);
}
