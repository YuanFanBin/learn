#include "apue.h"

/* 结合figure-15.18.c使用 */
/* gcc apue.h apue_err.c figure-15.19.c -o add2 */
int
main(void)
{
    int     int1, int2;
    char    line[MAXLINE];

    /* 若不增加如下内容，标准I/O使用缓冲机制，因为标准输入是一个管道，
     * 标准I/O库默认是全缓冲的，标准输出也是如此 */
    /* 强行更改成行缓冲模式 */
    /*
    if (setvbuf(stdin, NULL, _IOLBF, 0) != 0)       // #1
        err_sys("setvbuf error");                   // #2
    if (setvbuf(stdout, NULL, _IOLBF, 0) != 0)      // #3
        err_sys("setvbuf error");                   // #4
    */
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
