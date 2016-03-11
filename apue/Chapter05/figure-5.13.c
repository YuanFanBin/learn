#include "apue.h"
#include <stdlib.h>    /* mkstemp, */
#include <errno.h>     /* errno, ENOENT */
#include <sys/types.h> /* stat */
#include <sys/stat.h>  /* stat */
#include <unistd.h>    /* stat, unlink */

void make_temp(char *template);

/* gcc apue.h apue_err.c figure-5.12.c */
/* Single UNIX Specification为处理临时文件定义了另外两个函数：mkdtemp，mkdtemp
 * 它们是XSI的扩展部分 */
/* 原子性 */
int
main(int argc, char *argv[])
{
    /* 数据在栈上 */
    char  good_template[] = "/tmp/dirXXXXXX"; /* right way */
    /* 数据指针在栈上，数据在只读段上，无法修改 */
    char *bad_template = "/tmp/dirXXXXXX"; /* wrong way */
    printf("trying to create first temp file...\n");
    make_temp(good_template);
    printf("trying to create second temp file...\n");
    make_temp(bad_template);
    exit(0);
}

void
make_temp(char *template)
{
    int fd;
    struct stat sbuf;

    if ((fd = mkstemp(template)) < 0) {
        err_sys("Can't create temp file");
    }
    printf("temp name = %s\n", template);
    close(fd);
    if (stat(template, &sbuf) < 0) {
        if (errno == ENOENT) {
            printf("file doesn't exist\n");
        } else {
            printf("stat filed");
        }
    } else {
        printf("file exists\n");
        unlink(template); /* 不会自动删除，必须自己对它解除链接 */
    }
}
