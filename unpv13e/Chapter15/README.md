## 第十五章 Unix域套接字

#### 15.1 概述

使用Unix域套接字有以下3个理由：

- 在源自Berkeley的实现中，Unix域套接字往往比通信两端位于同一个主机的TCP套接字快出一倍。

- Unix域套接字可用于在同一个主机上的不同进程之间传递描述符。

- Unix域套接字较新的实现把客户的凭证（用户ID和组ID）提供给服务器，从而能够提供额外的安全检查措施。

#### 15.2 Unix域套接字地址结构

```c
#include <sys/un.h>

struct sockaddr_un {
    sa_family_t sun_family;     // AF_LOCAL
    char        sun_path[104];  // null-terminated pathname
}
```

例子：Unix域套接字的bind调用 [unixbind.c](unixbind.c)

```c
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/error.h"

#ifndef     SUN_LEN
# define    SUN_LEN(su) \
    (sizeof(*(su)) - sizeof((su)->sun_path) + strlen((su)->sun_path))
#endif

int main(int argc, char **argv)
{
    int                 sockfd;
    socklen_t           len;
    struct sockaddr_un  addr1, addr2;

    if (argc != 2) {
        err_quit("usage: unixbind <pathname>");
    }

    if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
        err_sys("socket error");
    }

    unlink(argv[1]);    // OK if this fails

    bzero(&addr1, sizeof(addr1));
    addr1.sun_family = AF_LOCAL;
    strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path) - 1);
    if (bind(sockfd, (struct sockaddr *) &addr1, SUN_LEN(&addr1)) == -1) {
        err_sys("bind error");
    }

    len = sizeof(addr2);
    if (getsockname(sockfd, (struct sockaddr *) &addr2, &len) == -1) {
        err_sys("getsockname error");
    }
    printf("bound name = %s returned len = %d\n", addr2.sun_path, len);

    exit(0);
}
```

```sh
$ gcc unixbind.c ../lib/error.c -o unixbind
$ ./unixbind /tmp/moose
bound name = /tmp/moose returned len = 13
```

#### 15.3 [socketpair](http://man7.org/linux/man-pages/man2/socketpair.2.html) 函数

#### 15.4 套接字函数

**详看原书内容**

#### 15.5 Unix域字节流客户/服务器程序

[unixstrserv01.c](unixstrserv01.c)

```c
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include "../lib/error.h"

#define UNIXSTR_PATH    "/tmp/unix.str"     /* Unix domain stream */
#define LISTENQ 1024            /* 2nd argument to listen() */

// Chapter05/str_echo.c
void str_echo(int sockfd);
// Chapter/05/sigchildwaitpid.c
void sig_chld(int);

int main(int argc, char **argv)
{
    int                 listenfd, connfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_un  cliaddr, servaddr;

    if ((listenfd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1)  {
        err_sys("socket error");
    }

    unlink(UNIXSTR_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        err_sys("bind error");
    }
    if (listen(listenfd, LISTENQ) == -1) {
        err_sys("listen error");
    }

    signal(SIGCHLD, sig_chld);

    for (;;) {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                err_sys("accept error");
            }
        }
        if ((childpid = fork()) == -1) {
            err_sys("fork error");
        } else if (childpid == 0) { // child
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
}
```

[unixstrcli01.c](unixstrcli01.c)

```c
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "../lib/error.h"

#define UNIXSTR_PATH    "/tmp/unix.str"     /* Unix domain stream */

// Chapter05/str_cli.c
void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_un  servaddr;

    if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        err_sys("connect error");
    }
    str_cli(stdin, sockfd);
    exit(0);
}
```

```sh
# 服务端
$ gcc unixstrserv01.c ../lib/error.c ../Chapter05/str_echo.c ../Chapter05/sigchildwaitpid.c -o unixstrserv01
$ ./unixstrserv01

# 客户端
$ gcc unixstrcli01.c ../lib/error.c ../Chapter03/readline.c ../Chapter05/str_cli.c -o unixstrcli01
$ ./unixstrcli01
hello           # input
hello           # output
```

#### 15.6 Unix与数据报客户/服务器程序

[unixdgserv01.c](unixdgserv01.c)

```c
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "../lib/error.h"

#define UNIXSTR_PATH    "/tmp/unix.str"     /* Unix domain stream */

// Chapter08/dg_echo.c
void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen);

int main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_un  servaddr, cliaddr;

    if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
        err_sys("socket error");
    }
    unlink(UNIXSTR_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);
    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        err_sys("bind error");
    }
    dg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}
```
[unixdgcli01.c](unixdgcli01.c)

```c
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/error.h"

#define UNIXSTR_PATH    "/tmp/unix.str"     /* Unix domain stream */

// Chapter08/dg_cli.c
void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen);

int main(int argc, char **argv)
{
    int     sockfd;
    struct sockaddr_un cliaddr, servaddr;

    if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
        err_sys("socket error");
    }
    // 在TCP，UDP协议中connect调用会给本套接字捆绑一个路径名
    // 但Unix域套接字不会主动捆绑一个路径名，我们需要手动绑定
    // 一个路径，否则Unix域套接字无法回射应答
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_LOCAL;
    strcpy(cliaddr.sun_path, tmpnam(NULL));
    if (bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0) {
        err_sys("bind error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    exit(0);
}
```

Unix域数据报客户与UDP客户不同，当使用Unix域数据报协议时，我们必须显示 *bind* 一个路径名到我们的套接字，这样服务器才会有能力回射应答的路径名。

```sh
# 服务端
$ gcc unixdgserv01.c ../lib/error.c ../Chapter08/dg_echo.c -o unixdgserv01
$ ./unixdgserv01


# 客户端
$ gcc unixdgcli01.c ../lib/error.c ../Chapter08/dg_cli.c -o unixdgcli01
[dg_cli.c]: sendto, recvfrom.
hello           # input
hello           # output
```

#### 15.7 描述符传递

当考虑从一个进程传递描述符到另一个进程时，我们通常会想到：

- *fork* 调用返回之后，子进程共享父进程的所有打开的描述符

- *exec* 调用执行之后，所有描述符通常保持打开状态不变

若想子进程打开传递一个描述符至父进程就无法实现，当前的Unix域套接字提供了用于从一个进程向其他进程传递任一打开的描述符方法。

两个进程之间传递描述符步骤如下：

- 创建一个字节流或数据报的Unix域套接字

- 发送进程通过调用返回描述符的任一Unix函数打开一个描述符（如：*open*, *pipe*, *mkfifo*, *socket*, *accept*）

- 发送进程创建一个 *msghdr* (14.5小节)结构，其中含有待传递的描述符，调用 *sendmsg* 发送描述符

- 接受进程调用 *recvmsg* 在来自步骤1的Unix域套接字上接受这个描述符

描述符传递的例子：

名为 *mycat* 的程序，它通过命令行参数取得一个路径名，打开这个文件，再把文件的内容复制到标准输出。

![使用socketpair创建流管道后的mycat进程](doc/figure-14-7.png)

![启动执行openfile程序后的mycat进程](doc/figure-14-8.png)

通过执行另一个程序来打开文件的优势在于，另一个程序可以是一个 *setuid* 到 *root* 的程序，能够打开我们通常没有打开权限的文件。

[mycat.c](mycat.c)

```c
#include <fcntl.h>
#include <unistd.h>
#include "../lib/error.h"

#define BUFFSIZE    8192        /* buffer size for reads and writes */

int my_open(const char *, int);

int main(int argc, char **argv)
{
    int     fd, n;
    char    buff[BUFFSIZE];

    if (argc != 2) {
        err_quit("usage: mycat <pathname>");
    }
    if ((fd = my_open(argv[1], O_RDONLY)) < 0) {
        err_sys("cannot open %s", argv[1]);
    }
    while ((n = read(fd, buff, BUFFSIZE)) > 0) {
        write(STDOUT_FILENO, buff, n);
    }
}
```

```sh
$ gcc openfile.c write_fd.c ../lib/error.c -o openfile
$ gcc mycat.c myopen.c read_fd.c ../lib/error.c -o mycat
$ ./mycat mycat.c
...         # 文件内容
```

#### 15.8 接受发送者的凭证

**需要使用时再了解**

#### 15.9 小结

Unix域套接字是客户和服务器在同一个主机上的IPC方法之一。与IPC其他方法相比，Unix域套接字的优势体现在其API几乎等同于网络客户/服务器使用的API。与客户和服务器在同一个主机上的TCP相比，Unix域字节流套接字的优势体现在性能的增长上。
