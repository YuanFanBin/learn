#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_SERVER "/tmp/fifoserver"

/* 进程间通信FIFO：http://blog.csdn.net/xiajun07061225/article/details/8471777 */
/* 
 * PIPE_BUF是FIFO的长度，它在头文件limits.h中被定义。
 * 在linux或其他类UNIX系统中，它的值通常是4096字节。
 */
/*
 * 把写程序编译成两个四个版本：
 *     非阻塞且请求写的字节数大于PIPE_BUF版本   : nbwg
 *     非阻塞且请求写的字节数不大于PIPE_BUF版本 : nbw
 *     阻塞且请求写的字节数大于PIPE_BUF版本     : bwg
 *     阻塞且请求写的字节数不大于PIPE_BUF版本   : bw
 */
/* gcc 15.5-1.c */
/* sudo ... */
int
main(int argc, char *argv[])
{
    int     fd;
    char    w_buf[4096*2];
    int     read_wnum;

    memset(w_buf, 0, 4096*2);
    if ((mkfifo(FIFO_SERVER, O_CREAT|O_EXCL) < 0) && (errno != EEXIST))
        fprintf(stdout, "can't create fifoserver\n");
    if (fd == -1 && errno == ENXIO)
        fprintf(stdout, "open error; no reading process\n");

    /* 设置非阻塞标志, nbw */
    //fd = open(FIFO_SERVER, O_WRONLY|O_NONBLOCK, 0);
    /* 设置阻塞标识, bw */
    fd = open(FIFO_SERVER, O_WRONLY, 0);
    read_wnum = write(fd, w_buf, 2048);
    if (read_wnum == -1) {
        if (errno == EAGAIN)
            fprintf(stdout, "write to fifo error; try later\n");
    } else {
        fprintf(stdout, "read write num is %d\n", read_wnum);
    }
    /* 5000用于测试写入字节大于4096时的非原子性, greater */
    //read_wnum = write(fd, w_buf, 5000);
    /* 4096 用于测试写入字节不大于4096时的原子性, not greater */
    read_wnum = write(fd, w_buf, 4096);

    if (read_wnum == -1 && errno == EAGAIN)
        fprintf(stdout, "try later\n");
}
/*
 * https://www.ibm.com/developerworks/cn/linux/l-ipc/part1/
 */
/*
 * 验证阻塞写操作：
 * 当请求写入的数据量大于PIPE_BUF时的非原子性：
 * nbr 1000
 * bwg
 * 当请求写入的数据量不大于PIPE_BUF时的原子性：
 * nbr 1000
 * bw
 * 验证非阻塞写操作：
 * 当请求写入的数据量大于PIPE_BUF时的非原子性：
 * nbr 1000
 * nbwg
 * 请求写入的数据量不大于PIPE_BUF时的原子性：
 * nbr 1000
 * nbw
 */
/* !!!!!!!!!!!
 *   不管写打开的阻塞标志是否设置，在请求写入的字节数大于4096时，
 * 都不保证写入的原子性。但二者有本质区别：
 *   对于阻塞写来说，写操作在写满FIFO的空闲区域后，会一直等待，
 * 直到写完所有数据为止，请求写入的数据最终都会写入FIFO；
 *   而非阻塞写则在写满FIFO的空闲区域后，就返回(实际写入的字节数)，
 * 所以有些数据最终不能够写入。
 */
