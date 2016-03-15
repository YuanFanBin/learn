#include "apue.h"
#include <syslog.h>
#include <errno.h>

extern int lockfile(int);
extern int already_running(void);

void
reread(void)
{
    /* ... */
}

void
sigterm(int signo)
{
    /* sudo kill -TERM xxxx */
    syslog(LOG_INFO, "got SIGTERM; exiting");
    syslog(LOG_EMERG, "got SIGTERM; exiting"); // 这个才能打出日志
    exit(0);
}

void
sighup(int signo)
{
    /* sudo kill -HUP xxxx */
    syslog(LOG_INFO, "Re-reading configuration file");
    syslog(LOG_EMERG, "Re-reading configuration file"); // 这个才能打出日志
    reread();
}

/* gcc 13.6-2.c 13.5.c 13.3.c apue.h apue_err.c */
int
main(int argc, char *argv[])
{
    char             *cmd;
    struct sigaction  sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;

    /* 
     * Become a daemon.
     */
    daemonize(cmd);

    /*
     * Make sure only once copy of the daemon is running.
     */
    if (already_running()) {
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }

    /*
     * Handle signals of interest.
     */
    sa.sa_handler = sigterm;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGHUP);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, NULL) < 0) {
        syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
        exit(1);
    }
    sa.sa_handler = sighup;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGTERM);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        syslog(LOG_ERR, "can't catch SIGHUP: %s", strerror(errno));
        exit(1);
    }

    /*
     * Proceed with the rest of the daemon.
     */
    /* ... */
    syslog(LOG_EMERG, "LOG_EMERG can print log"); // 这个才能打出日志
    /* 这样就能看到守护进程了 */
    for (;;) {
    }
    exit(0);
}

/* syslog ==> /var/log/messages */
/* 1. /var/log/lastlog : 记录每个使用者最近签入系统的时间, 因此当使用者签入时, 
 *   就会显示其上次签入的时间, 您应该注意一下这个时间, 若不是您上次签入的时间, 
 *   表示您的帐号可能被人盗用了. 此档可用 /usr/bin/lastlog 指令读取.
 * 2. /var/run/utmp : 记录每个使用者签入系统的时间, who, users, finger 等指令
 *   会查这个档案.
 * 3. /var/log/wtmp : 记录每个使用者签入及签出的时间, last 这个指令会查这个档
 *   案. 这个档案也记录 shutdown 及 reboot 的动作.
 * 4. /var/log/secure : 登录系统的信息
 * 5. /var/log/maillog : 记录 sendmail 及 pop 等相关讯息.
 * 6. /var/log/cron : 记录 crontab 的相关讯息 ，定时器的信息
 * 7. /var/log/dmesg : /bin/dmesg 会将这个档案显示出来, 它是开机时的画面讯息.
 * 8. /var/log/xferlog : 记录那些位址来 ftp 拿取那些档案.
 * 9. /var/log/messages : 系统大部份的讯息皆记录在此, 包括 login, 
 *   check password , failed login, ftp, su 等.
 */
/* 参考资料：http://blog.chinaunix.net/uid-25120309-id-3359929.html */

/* 参考资料：http://www.cnblogs.com/Anker/archive/2012/12/20/2825929.html */
