#include "apue.h"
#include <stdio.h> /* L_tmpname, tmpnam, rewind, TMP_MAX */

/* gcc apue.h apue_err.c figure-5.12.c */
int
main(int argc, char *argv[])
{
    char name[L_tmpnam], line[MAXLINE];
    FILE *fp;

    printf("%s\n", tmpnam(NULL)); /* first temp name */

    /* tmpnam最多调用TMP_MAX次 */
    /* tmpnam创建文件名 */
    tmpnam(name); /* second temp name */
    printf("%s\n", name);
    printf("TMP_MAX = %d\n", TMP_MAX);

    /* tmpfile创建文件 */
    if ((fp = tmpfile()) == NULL) { /* create temp file */
        err_sys("tmpfile error");
    }
    fputs("one line of output\n", fp); /* write to temp file */
    rewind(fp); /* then read it back */
    if (fgets(line, sizeof(line), fp) == NULL) {
        err_sys("fgets error");
    }
    fputs(line, stdout); /* print the line we wrote */
    
    exit(0);
}
