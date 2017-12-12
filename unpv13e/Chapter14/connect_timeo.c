#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../lib/error.h"

static void connect_alarm(int);

typedef void Sigfunc(int);

int connect_timeo(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    Sigfunc *sigfunc;
    int      n;

    // 设置新的信号中断处理函数，并保留原有中断处理函数
    sigfunc = signal(SIGALRM, connect_alarm);
    if (alarm(nsec) != 0) {
        err_msg("connect_timeo: alarm was already set");
    }
    if ((n = connect(sockfd, saptr, salen)) < 0) {
        close(sockfd);
        if (errno == EINTR) { // 产生中断错误
            errno = ETIMEDOUT;
        }
    }
    alarm(0); // turn off the alarm
    signal(SIGALRM, sigfunc); // restore previous signal handler
    return(n);
}

static void connect_alarm(int signo)
{
    return; // just interrupt the connect()
}
