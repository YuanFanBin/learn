#include <stdio.h>

#define MAXLINE 4096 /* max text line length */

void str_echo(int sockfd)
{
    char  line[MAXLINE];
    FILE *fpin, *fpout;

    fpin = fdopen(sockfd, "r");
    fpout = fdopen(sockfd, "w");

    while (fgets(line, MAXLINE, fpin) != NULL) {
        // 标准I/O中函数有读写buffer问题（完全缓冲，参考APUE相关章节）
        // fputs先写buffer，buffer未满前数据并没有实际写入fpout
        //
        // 当我们关闭server时，exit函数会关闭所有已打开文件描述符
        // 此时数据才从标准IO的缓冲区写至fpout（buffer未满情况下）
        fputs(line, fpout);
    }
}
