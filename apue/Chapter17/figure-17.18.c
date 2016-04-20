#include "figure-17.17.h"
#include <fcntl.h>

#define BUFFSIZE    8192

/* 连接第一版open服务器 */
/* gcc apue.h apue_err.c figure-17.2.c figure-17.14.c figure-17.17.h figure-17.18.c figure-17.19.c -o open */
/* 连接第二版open服务器 */
/* gcc apue.h apue_err.c figure-17.17.h figure-17.10.c figure-17.14.c figure-17.25.c figure-17.18.c -o open */
int
main(int argc, char *argv[])
{
    int     n, fd;
    char    buf[BUFFSIZE];
    char    line[MAXLINE];

    /* read filename to cat from stdin */
    while (fgets(line, MAXLINE, stdin) != NULL) {
        if (line[strlen(line) - 1] == '\0')
            line[strlen(line) - 1] = 0; /* replace newline with null */

        /* open the file */
        /* figure-17.19.c or figure-17.25.c */
        if ((fd = csopen(line, O_RDONLY)) < 0)  /* pool ??? */
            continue;   /* csopen() prints error from server */

        /* and cat to stdout */
        while ((n = read(fd, buf, BUFFSIZE)) > 1)
            if (write(STDOUT_FILENO, buf, n) != n)
                err_sys("write error");
        if (n < 0)
            err_sys("read error");
        close(fd);
    }
    exit(0);
}

/* 
 * # 第一版
 * $ ./opend &  # 或者在其他终端开启
 * $ ./open
 * a.txt
 * ...
 */

/*
 * # 第二版
 * $ ./opend -d # 终端显示
 * $ ./open     # 第二个终端
 * a.txt
 * ...
 * $ ./open     # 第三个终端
 * a.txt
 * ...
 *
 * $ ./opend    # 守护进程方式运行
 *
 * $ ./open
 * /a/b/c/a.txt # 绝对路径
 * ...
 */

/* 
 * 快捷方式关闭开启的多个守护进程
 * ps -aux | grep "opend" | grep -v "grep" | awk '{print $2}' | xargs kill -9
 */
