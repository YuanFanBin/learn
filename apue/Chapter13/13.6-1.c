#include "apue.h"
#include <pthread.h>
#include <syslog.h>

sigset_t mask;

/* 13.5.c */
extern int already_running(void);

void
reread(void)
{
    /* ... */
}

void *
thr_fn(void *arg)
{
    int err, signo;

    for (;;) {
        err = sigwait(&mask, &signo);
        if (err != 0) {
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }
        switch(signo) {
        case SIGHUP: /* sudo kill -HUP xxxx */
            syslog(LOG_INFO, "Re-reading configuration file");
            reread();
            break;

        case SIGTERM: /* sudo kill -TERM xxxx */
            syslog(LOG_INFO, "got SIGTERM; exiting");
            exit(0);

        default:
            syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }
    }
    return(0);
}

/* gcc 13.6-1.c 13.5.c 13.3.c apue.h apue_err.c -lpthread */
int
main(int argc, char *argv[])
{
    int               err;
    pthread_t         tid;
    char             *cmd;
    struct sigaction  sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;

    /*
     * Become a daemon.
     */
    daemonize(cmd); /* 13.3.c */

    /*
     * Make sure only one copy of the daemon is running.
     */
    if (already_running()) {
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }

    /*
     * Restore SIGHUP default and block all signals.
     */
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_quit("%s: can't restore SIGHUP default");
    sigfillset(&mask);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
        err_exit(err, "SIG_BLOCK error");

    /*
     * Create a thread to handle SIGHUP and SIGTERM.
     */
    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0)
        err_exit(err, "can't create thread");

    syslog(LOG_INFO, "PID = %ld", (long)getpid());
    /*
     * Proceed with the rest of the daemon.
     */
    /* ... */
    for (;;) {
    }
    exit(0);
}


/* 多线程守护进程 */
/* 样例为守护进程可以重读其配置文件的一种方法。*/

/*
 * 守护进程的惯例：
 * 1.若守护进程使用锁文件，那么该文件通常存储在/var/run目录下。
 * 2.若守护进程支持配置选项，那么配置文件通常存放在/etc目录下。
 * 3.守护进程可用命令行启动，但通常它们是系统初始化脚本之一
 *   (/etc/rc*或/etc/init.d/*启动的。)
 * 4.若一个守护进程有一个配置文件，那么当该文件启动时会读该文件，
 *   但在此之后一般就不会再查看它。
 */
