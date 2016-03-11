#第五章
---
对标准I/O库，它们的操作是围绕流(stream)进行的。
##缓冲
1.全缓冲：填满标准I/O缓冲区后才进行实际I/O操作；
2.行缓冲：当输入和输出中遇到换行符是，标准I/O执行I/O操作；
3.不带缓冲：标准I/O库不对字符进行缓冲存储。

很多系统默认使用下列类型的缓冲：
*标准错误是不带缓冲的
*若是指向终端设备的流，则是行缓冲的；否则是全缓冲的。

代码样例(5.4)：
```c
#include <stdio.h>
#define BUFSIZE 1024
int main(int argc, char *argv[])
{
    int ret;
    char buf[BUFSIZE];

    fprintf(stderr, "-----------------------\n");
    fprintf(stdout, "<terminal>io line buf, waiting\\n...");
    fprintf(stderr, "waiting 2 second.\n");
    sleep(2);
    fprintf(stdout, " <terminal> line buf\n");
    sleep(2);
    setbuf(stdout, NULL); /* 设为非缓冲模式，即时输出信息 */
    fprintf(stdout, "io no buf, now!");
    sleep(2);
    fprintf(stdout, "\\n test\n");
    sleep(2);

    fprintf(stderr, "-----------------------\n");
    /* 终端下stderr为不带缓冲模式，修改为行缓冲 */
    ret = setvbuf(stderr, buf, _IOLBF, 1024);
    if (ret == -1) {
        fprintf(stderr, "setvbuf failed.\n");
    }
    fprintf(stderr, "waiting 2 second.");
    sleep(2);
}
```
