#include "apue.h"
#include <ctype.h>

/* gcc 15.3-3.c apue.h apue_err.c -o myuclc */
/* 转义大写 */
int
main(void)
{
    int     c;

    while ((c = getchar()) != EOF) {
        if (isupper(c))
            c = tolower(c);
        if (putchar(c) == EOF)
            err_sys("output error");
        if (c == '\n')
            fflush(stdout);
    }
    exit(0);
}
