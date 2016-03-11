#include "apue.h"

/* 结合15.4-2.c使用 */
/* gcc 15.4-3.c apue.h apue_err.c -o add2 */
int
main(void)
{
    int     int1, int2;
    char    line[MAXLINE];

    /* 若不增加如下内容，标准I/O使用缓冲机制，因为标准输入是一个管道，
     * 标准I/O库默认是全缓冲的，标准输出也是如此 */
    /* 强行更改成行缓冲模式 */
    if (setvbuf(stdin, NULL, _IOLBF, 0) != 0)
        err_sys("setvbuf error");
    if (setvbuf(stdout, NULL, _IOLBF, 0) != 0)
        err_sys("setvbuf error");
    while(fgets(line, MAXLINE, stdin) != NULL) {
        if (sscanf(line, "%d%d", &int1, &int2) == 2) {
            if (printf("%d\n", int1 + int2) == EOF)
                err_sys("printf error");
        } else {
            if (printf("invalid args\n") == EOF)
                err_sys("printf error");
        }
    }
    exit(0);
}
