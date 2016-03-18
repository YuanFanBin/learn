#include "apue.h"
#include <stdlib.h> /* getenv, putenv */
#include <unistd.h> /* environ */

/* gcc apue.h apue_err.c figure-7.4.c */
/* 7.4 & 7.5 */
extern char **environ;
/* 1 基本解释：extern可以置于变量或者函数前，以标示变量或者函数的定义
 * 在别的文件中，提示编译器遇到此变量和函数时在其他模块中寻找其定义。
 * 此外extern也可用来进行链接指定 */
/* http://www.cnblogs.com/yc_sunniwell/archive/2010/07/14/1777431.html */
int
main(int argc, char *argv[])
{
    int i;
    //for (i = 0; i < argc; i++) {
    /* ISO C, POSIX.1都要求argv[argc]是一个空指针，因此可以这么写 */
    for (i = 0; argv[i] != NULL; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    for (i = 0; environ[i] != NULL; i++) {
        printf("environ[%d]: %s\n", i, environ[i]);
    }
    /* 通常使用getenv, putenv访问特定的环境变量 */
    printf("HOSTNAME = %s\n", getenv("HOSTNAME"));
    exit(0);
}
