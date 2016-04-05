#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

/* figure-14.9.c */
extern int lockfile(int);
/* 参考资料：http://blog.csdn.net/cxxmaker/article/details/7416136 */

int
already_running(void)
{
    int     fd;
    char    buf[16];

    fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
    if (fd < 0) {
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    if (lockfile(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return(1);
        }
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    /* 需要将文件长度截断为0，其原因是之前的守护进程实例的进程ID字符串可能长于
     * 调用此函数的当前进程的进程ID字符串。
     * 例如：
     *  若以前的守护进程ID是12345，而新实例的进程ID是999，那么将此进程ID写入文
     *  件后，在文件中留下的是99995。将文件阶段为0就解决了此问题。
     */
    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    return(0);
}
