#include "../lib/error.h"
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);

int my_open(const char *pathname, int mode)
{
    int     fd, sockfd[2], status;
    pid_t   childpid;
    char    c, argsockfd[10], argmode[10];

    if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0) {
        err_sys("socketpair error");
    }
    if ((childpid = fork()) < 0) {
        err_sys("fork error");
    } else if (childpid == 0) { // child
        close(sockfd[0]);
        snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
        snprintf(argmode, sizeof(argmode), "%d", mode);
        execl("./openfile", "openfile", argsockfd, pathname, argmode, (char *) NULL);
        err_sys("execl error)");
    }
    close(sockfd[1]);
    waitpid(childpid, &status, 0);
    if (WIFEXITED(status) == 0) {
        err_quit("child did not terminate");
    }
    if ((status = WEXITSTATUS(status)) == 0) {
        // 通过流管道发送和接收描述符时，我们总是发送至少1个字节的数据，
        // 即便接收进程不对数据做任何处理。
        // 要是不这么做，接收进程难以辨别read_fd的返回值为0意味着“没有
        // 数据（但是可能伴有一个描述符）”还是”文件已结束“
        read_fd(sockfd[0], &c, 1, &fd);
    } else {
        errno = status;
        fd = -1;
    }
    close(sockfd[0]);
    return(fd);
}
