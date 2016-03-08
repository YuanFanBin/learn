#include "apue.h"

/* gcc apue.h apue_er.c figure-4.24.c figure-2.16.c */
int
main(void)
{
    char    *ptr;
    size_t   size;

    if (chdir("/usr/spool/cron") < 0)
        err_sys("chdir failed");
    ptr = path_alloc(&size);    /* {(Figure-2.16.c)} */
    if (getcwd(ptr, size) == NULL)
        err_sys("getcwd failed");
    printf("cwd = %s\n", ptr);
    exit(0);
}
