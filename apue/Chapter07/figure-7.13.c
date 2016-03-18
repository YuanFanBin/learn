#include "apue.h"
#include <setjmp.h> /* setjmp, longjmp */

static void f1(int, int, int, int);
static void f2(void);

static jmp_buf jmpbuffer;
static int     globval;

/* gcc apue.h apue_err.c figure-7.13.c */
/* 编译优化：gcc -O apue.h apue_err.c figure-7.13.c */
/* 通过此例，若要编写一个使用非局部变量跳转的可移植程序，则必须使用
 * volatile属性 */
int
main(void)
{
    int          autoval;
    register int regival;
    /* 参考资料：http://blog.csdn.net/tigerjibo/article/details/7427366 */
    volatile int volaval;
    static int   statval;
    int          ret;

    globval = 1; autoval = 2; regival = 3; volaval = 4; statval = 5;

    if ((ret = setjmp(jmpbuffer)) != 0) {
        printf("ret = %d\n", ret);
        printf("after longjmp:\n");
        printf("global = %d, autoval = %d, regival = %d,"
               " volaval = %d, statval = %d\n",
               globval, autoval, regival, volaval, statval);
        exit(0);
    }

    /*
     * Change variables after setjmp, but before longjmp.
     */
    globval = 95; autoval = 96; regival = 97; volaval = 98; statval = 99;

    f1(autoval, regival, volaval, statval); /* never returns */
    exit(0);
}

static void
f1(int i, int j, int k, int l)
{
    printf("in f1():\n");
    printf("globval = %d, autoval = %d, regival = %d,"
           " volaval = %d, statval = %d\n", globval, i, j, k, l);
    f2();
}

static void
f2(void)
{
    /* 第二个参数为setjmp的返回值，通过控制此参数可对于不同的处理方式 */
    longjmp(jmpbuffer, 5);
}
