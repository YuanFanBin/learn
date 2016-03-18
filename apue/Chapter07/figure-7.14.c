#include <stdio.h>

FILE *
open_data(void)
{
    FILE   *fp; /* 栈空间，函数退出时，对应的数据空间会被下一个函数的栈帧使用 */
    char    databuf[BUFSIZ];    /* setvbuf makes this the stdio buffer */

    if ((fp = fopen("datafile", "r")) == NULL)
        return(NULL);
    if (setvbuf(fp, databuf, _IOLBF, BUFSIZ) != 0)
        return(NULL);
    return(fp); /* error */
}
