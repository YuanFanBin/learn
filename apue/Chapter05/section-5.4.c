#include <stdio.h>

#define BUFSIZE 1024

/* gcc section-5.4.c */
int main(int argc, char *argv[])
{
    int ret;
    char buf[BUFSIZE];

    fprintf(stderr, "-----------------------\n");               // #1
    fprintf(stdout, "<terminal>io line buf, waiting\\n...");    // #2
    fprintf(stderr, "waiting 2 second.\n");                     // #3
    sleep(2);                                                   // #4
    fprintf(stdout, " <terminal> line buf\n");                  // #5
    sleep(2);                                                   // #6
    setbuf(stdout, NULL); /* 设为非缓冲模式，即时输出信息 */    // #7
    fprintf(stdout, "io no buf, now!");                         // #8
    sleep(2);                                                   // #9
    fprintf(stdout, "\\n test\n");                              // #10
    sleep(2);                                                   // #11

    fprintf(stderr, "-----------------------\n");               // #12
    /* 终端下stderr为不带缓冲模式，修改为行缓冲 */
    ret = setvbuf(stderr, buf, _IOLBF, 1024);                   // #13
    if (ret == -1) {
        fprintf(stderr, "setvbuf failed.\n");

    }
    fprintf(stderr, "waiting 2 second.");                       // #14
    sleep(2);                                                   // #15
    exit(0);                                                    // #16
}
