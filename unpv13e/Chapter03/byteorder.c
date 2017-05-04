#include <stdio.h>  /* printf */
#include <stdlib.h> /* exit */

/* gcc byteorder.c */
int main(int argc, char **argv)
{
    union {
        short   s;
        char    c[sizeof(short)];
    } un;
    un.s = 0x0102;
    // printf("%s: ", CPU_VENDOR_OS); /* autoconf 生成 */
    if (sizeof(short) == 2) {
        if (un.c[0] == 1 && un.c[1] == 2) {
            printf("big-endian\n");
        } else if (un.c[0] == 2 && un.c[1] == 1) {
            printf("litte-endian\n");
        } else {
            printf("unknown\n");
        }
    } else {
        printf("sizeof(short) = %d\n", (int)sizeof(short));
    }
    exit(0);
}