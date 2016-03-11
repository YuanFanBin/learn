#include "apue.h"
#include <sys/times.h>

/* 1.10节中说明我们可以度量的3个时间：
 * 墙上时钟时间、用户CPU时间、系统CPU时间。
 * 任一进程都可使用times函数获得它自己以及终止子进程的上数值。
 */

static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

/* gcc 8.17.c apue.h apue_err.c */
int
main(int argc, char *argv[])
{
    int i;

    setbuf(stdout, NULL);
    for (i = 1; i < argc; i++)
        do_cmd(argv[i]); /* once for each command-line arg */
    exit(0);
}

static void
do_cmd(char *cmd) /* execute and time the "cmd" */
{
    struct tms   tmsstart, tmsend;
    clock_t      start, end;
    int          status;

    printf("\ncommand: %s\n", cmd);
    if ((start = times(&tmsstart)) == -1) /* starting values */
        err_sys("times error");

    if ((status = system(cmd)) < 0) /* execute command */
        err_sys("system() error");

    if ((end = times(&tmsend)) == -1) /* ending values */
        err_sys("times error");

    pr_times(end - start, &tmsstart, &tmsend);
    pr_exit(status);
}

/*
 * struct tms {
 *   clock_t tms_utime;  // user CPU time
 *   clock_t tms_stime;  // system CPU time
 *   clock_t tms_cutime; // user CPU time, terminated children
 *   clock_t tms_cstime; // system CPU time, terminated children
 * }
 */
static void
pr_times(clock_t real, struct tms *tmsstart, struct tms * tmsend)
{
    /* 机智 */
    static long  clktck = 0;

    if (clktck == 0) /* fetch clock ticks per second first time */
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
            err_sys("sysconf = error");

    printf("  real:  %7.2f\n", real / (double)clktck); /* /时钟滴答数 */
    printf("  user:  %7.2f\n", 
           (tmsend->tms_utime - tmsstart->tms_utime) / (double)clktck);
    printf("  sys :  %7.2f\n",
           (tmsend->tms_stime - tmsstart->tms_stime) / (double)clktck);
    printf("  child user:  %7.2f\n",
           (tmsend->tms_cutime - tmsstart->tms_cutime) / (double)clktck);
    printf("  child sys :  %7.2f\n",
           (tmsend->tms_cstime - tmsstart->tms_cstime) / (double)clktck);
}

/* 8.6-1.c */
void
pr_exit(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termiantion, signal number = %d%s\n\n",
                WTERMSIG(status),
#ifdef WCOREDUMP
                WCOREDUMP(status) ? " (core file generated)" : "");
#else
                "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stopped, signal number = %d\n\n", WSTOPSIG(status));
    }
}
