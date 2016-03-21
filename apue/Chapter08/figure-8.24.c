#include "apue.h"

/* gcc apue.h apue_err.c figure-8.5.c figure-8.24.c -o tsys */
/* 需 figure-8.25 -o printuids 配合 */
int
main(int argc, char *argv[])
{
    int     status;

    if (argc < 2)
        err_quit("command-line argument required");

    if ((status = system(argv[1])) < 0)
        err_sys("system() error");

    pr_exit(status);

    exit(0);
}

/*
 * $ tsys printuids     # 正常执行，无特权
 * $ su                 # 成为超级用户
 * # chown root tsys    # 更改所有者
 * # chmod u+s tsys     # 增加设置用户ID [4.5 文件访问权限](#xx)
 * # ls -l tsys
 * # exit
 * $ tsys printuids
 */
