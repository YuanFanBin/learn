#include "apue.h"

/* gcc apue.h figure-3.1.c */
int
main(void)
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        printf("cann't seek\n");
    else
        printf("seek OK\n");
    exit(0);
}
