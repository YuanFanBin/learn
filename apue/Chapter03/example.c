#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include "apue.h"

int main(int argc, char *argv[]) {
    int             fd;
    struct flock    fl;

    if ((fd = open("testfile", O_RDWR) == -1)) {
        err_sys("open 'testfile' fail");
    }
    // 对文件 100 - 109 字节加锁
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 100;
    fl.l_len = 10;

    if (fcntl(fd, F_SETLK, &fl) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
            printf("already locked by another process\n");
        } else {
            err_sys("fcntl fail");
        }
    } else { // 加锁成功
        // 操作加锁内容
        // ...
        // 解锁加锁内容
        fl.l_type = F_UNLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 100;
        fl.l_len = 10;
        if (fcntl(fd, F_SETLK, &fl) == -1) {
            err_sys("fcntl fail");
        }
    }
    exit(EXIT_SUCCESS);
}

// 样例来源： man fcntl - Locking and Unlocking a File
