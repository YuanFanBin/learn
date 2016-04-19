#include "apue.h"
#include <sys/socket.h>

#define MAXSLEEP 128

int
connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen)
{
    int     numsec;

    /*
     * Try to connect with exponential backoff.
     */
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
        if (connect(sockfd, addr, alen) == 0) {
            /*
             * Connection accepted.
             */
            return(0);
        }

        /*
         * Delay before trying again.
         */
        if (numsec <= MAXSLEEP / 2)
            sleep(numsec);
    }
    return(-1);
}

/*
 * 指数补偿算法可参考：
 * https://en.wikipedia.org/wiki/Exponential_backoff
 *
 * 连接失败是由瞬时引起的，要想连接成功目标服务器需开启，并且正在运行，
 * 服务器必须绑定到一个想与之连接的地址上，并且服务器的等待连接队列要
 * 有足够的空间。
 *
 * 当连接失败，我们可以用如上提供的补偿算法，重试连接。
 */

/*
 * 以上代码虽然可用，但仍存在问题：代码是不可移植的。它在 Linux 和 
 * Solaris 上可以工作，但是在 FreeBSD 和 Mac OS X 上却不能按预期工作。
 * 在基于 BSD 的套接字实现中，如果第一次连接尝试失败，那么在 TCP 
 * 中继续使用同一个套接字描述符，接下来仍旧会失败。这就是一个协议相关
 * 的行为从（协议无关的）套接字接口中显露出来变得应用程序可见的例子。
 *
 * 为了解决这个问题可以使用figure-16.11.c提供的方法。
 */
