# APUE学习笔记

--------------------------------------------------------------------------------
目录
================================================================================
* [待办事宜](#待办事宜)
* [说明](#说明)
* [快捷命令](#快捷命令)
* [第一章 UNIX基础知识](#第一章-unix基础知识)
    * [1.4 文件及目录](#14-文件及目录)
    * [1.5 输入和输出](#15-输入和输出)
    * [1.6 程序和进程](#16-程序和进程)
    * [1.7 出错处理](#17-出错处理)
    * [1.8 用户标识](#18-用户标识)
    * [1.9 信号](#19-信号)
* [第二章 UNIX标准及实现](#第二章-unix标准及实现)
    * [2.5.4 函数sysconf,pathconf和fpathconf](#254-函数sysconfpathconf和fpathconf)
    * [2.5.5 不确定运行时限制](#255-不确定运行时限制)
    * [2.6 选项](#26-选项)
    * [2.8 基本系统数据类型](#28-基本系统数据类型)
* [第三章 文件I/O](#第三章-文件io)
    * [3.6 函数lseek](#36-函数lseek)
    * [3.9 I/O的效率](#39-io的效率)
    * [3.11 原子操作](#311-原子操作)
    * [3.14 函数fcntl](#314-函数fcntl)
    * [3.17 小结](#317-小结)
* [第四章 文件和目录](#第四章-文件和目录)
    * [4.2 函数stat，fstat，fstatat和lstat](#42-函数statfstatfstatat和lstat)
* [第十六章 网络IPC：套接字](#第十六章-网络ipc套接字)
    * [16.2 套接字描述符](#162-套接字描述符)
    * [16.3 寻址](#163-寻址)
        * [16.3.3 地址查询](#1633-地址查询)
            * [Figure-16.9.c](#figure-169c)
    * [16.4 建立连接](#164-建立连接)
        * [Figure-16.10.c](#figure-1610c)
        * [Figure-16.11.c](#figure-1611c)
        * [Figure-16.12.c](#figure-1612c)
    * [16.5 数据传输](#165-数据传输)
        * [Figure-16.16.c（面向连接的客户端）](#figure-1616c面向连接的客户端)
        * [Figure-16.17.c（面向连接的服务器）](figure-1617c面向连接的服务器)
        * [Figure-16.18.c](#figure-1618c)
        * [Figure-16.19.c（无连接的客户端）](#figure-1619c无连接的客户端)
        * [Figure-16.20.c（无连接的服务器）](#figure-1620c无连接的服务器)
    * [16.6 套接字选项](#166-套接字选项)
        * [Figure-16.22.c](#figure-16222c)
    * [习题](#习题)
        * [Figure-C.22.c](#figure-c22c)
* [第十七章 高级进程间通信](#第十七章-高级进程间通信)
    * [17.1 引言](#171-引言)
    * [17.2 UNIX域套接字](#172-unix域套接字)
        * [Figure-17.2.c](#figure-172c)
        * [Figure-17.3.c](#figure-173c)
        * [Figure-17.4.c](#figure-174c)
    * [命令UNIX域套接字](#命令unix域套接字)
        * [Figure-17.5.c](#figure-175c)
* [附录B 其他源代码](#附录b-其他源代码)
    * [B1 本书使用的头文件](#b1-本书使用的头文件)
        * [apue.h](#apueh)
    * [B2 标准出错例程](#b2-标准出错例程)
        * [apue_err.c](#apueerrc)
        * [apue_log.c](#apuelogc)

--------------------------------------------------------------------------------
说明
================================================================================

学习所使用系统：CentOS6.5

以下所有内容涉及的头文件及相应函数建议自行使用 `man` 查询，但本文也会给出相应头文件及函数在线man的查询链接。

[BACK TO TOP](#目录)

--------
快捷命令
========
`CTRL + D`：键入文件结束符 `^D`
`CTRL + C`：中断键 `^C`
`CTRL + \`：退出键 `^\`

[BACK TO TOP](#目录)

-------------------
第一章 UNIX基础知识
===================

## 1.4 文件及目录

### Figure-1.3.c
功能：列出指定目录下的所有文件名称， `ls` 的简要实现。
涉及头文件：[dirent.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/dirent.h.html)
涉及函数：[opendir](http://man7.org/linux/man-pages/man3/opendir.3.html)
沉淀内容：`opendir` 为我们提供一种在C程序中访问目录下文件的方式。

```c
#include "apue.h"
#include <dirent.h>

/* gcc apue.h apue_err.c figure-1.3.c */
int
main(int argc, char *argv[])
{
    DIR             *dp;
    struct dirent   *dirp;

    if (argc != 2)
        err_quit("usage: ls directory_name");

    if ((dp = opendir(argv[1])) == NULL)
        err_sys("can't open %s", argv[1]);
    while ((dirp = readdir(dp)) != NULL)
        printf("%s\n", dirp->d_name);

    closedir(dp);
    exit(0);
}
```

[BACK TO TOP](#目录)

## 1.5 输入和输出

不带缓冲的I/O，标准I/O。

### Figure-1.4.c

功能：利用不带缓冲的I/O简易文件复制。
涉及头文件：[unistd.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html)
涉及函数：[read](http://man7.org/linux/man-pages/man2/read.2.html)，[write](http://man7.org/linux/man-pages/man2/write.2.html)
沉淀内容：理解标准输入、标准输出及函数 `read`, `write` 的使用。

```c
#include "apue.h"

#define BUFFSIZE    4096

/* gcc apue.h apue_err.c figure-1.4.c */
int
main(void)
{
    int     n;
    char    buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");

    if (n < 0)
        err_sys("read error");

    exit(0);
}
```

在头文件 `unistd.h` 中包含了两个常量 *STDIN_FILENO* 和 *STDOUT_FILENO* 并且是 **POSIX** 的一部分，以上两个常量指定了标准输入和标准输出的文件描述符，在 **POSIX** 中定义为 0 和 1；标准错误常量为 *STDERR_FILENO* ，定义为 2。

### Figure-1.5.c

功能：利用标准I/O实现简单文件复制。
涉及头文件：[stdio.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdio.h.html)
涉及函数：[getc](http://man7.org/linux/man-pages/man3/getc.3.html)，[putc](http://man7.org/linux/man-pages/man3/putc.3.html)，[ferror](http://man7.org/linux/man-pages/man3/ferror.3.html)
沉淀内容：理解标准I/O的标准输入流 *stdin*，标准输出流 *stdout*，标准错误流 *stderr*。

```c
#include "apue.h"

/* gcc apue.h apue_err.c figure-1.5.c */
int
main(void)
{
    int     c;

    while ((c = getc(stdin)) != EOF)
        if (putc(c, stdout) == EOF)
            err_sys("output error");

    if (ferror(stdin))
        err_sys("input error");

    exit(0);
}
```

标准I/O函数为那些不带缓冲的I/O函数提供了一个带缓冲的接口。使用标准I/O函数无需担心如何选取最佳的缓冲区大小，还能简化对输入航的处理。

[BACK TO TOP](#目录)

## 1.6 程序和进程

### Figure-1.6.c

功能：打印进程ID。
涉及头文件：[unistd.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html)，[sys/types.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_types.h.html)
涉及函数：[getpid](http://man7.org/linux/man-pages/man2/getpid.2.html)
沉淀内容：如何利用api获取当前进程的进程ID，了解常用 `xxx_t` 类型的头文件。

```c
#include "apue.h"

/* gcc apue.h apue_err.c figure-1.6.c */
int
main(void)
{
    printf("hello world from process ID %ld\n", (long)getpid());
    exit(0);
}
```
虽然大多数进程ID可以用整型表示，但使用 *long* 可以提高移植性。

### Figure-1.7.c

功能：简易命令提示符的shell。
涉及头文件：[unistd.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html)，[sys/wait.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_wait.h.html)
涉及函数：[fork](http://man7.org/linux/man-pages/man2/fork.2.html)，[execlp](http://man7.org/linux/man-pages/man3/execlp.3.html)
沉淀内容：初步了解 `fork` 函数及 `exec` 族函数，学会如何编写一个简单的交互模式的进程。

```c
#include "apue.h"
#include <sys/wait.h>

/* gcc apue.h apue_err.c figure-1.7.c */
int
main(void)
{
    char    buf[MAXLINE];   /* from apue.h */
    pid_t   pid;
    int     status;

    printf("%% ");          /* print prompt (printf requires %% to print %) */
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;   /* replace newline with null */

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {      /* child */
            execlp(buf, buf, (char *)0);
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");
        printf("%% ");
    }
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out 
% ps
  PID TTY          TIME CMD
  611 pts/22   00:00:00 a.out
  612 pts/22   00:00:00 ps
28202 pts/22   00:00:01 bash
% pwd
/home/fanbin/learn/apue
% ^C
[fanbin@localhost apue]$ 
```

[BACK TO TOP](#目录)

## 1.7 出错处理

**当UNIX系统出错时，通常会返回一个负值，而且整型变量 *errno* 通常被设置为具有特定信息的值。** 文件 [`<errno.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html) 中定义了 *errno* 以及可以赋予它的各种常量，这些常量都以字符E开头。

**对于 *errno* 应当注意两条规则：
第一条规则是：如果没有出错，其值不会被例程清除。因此，仅当函数的返回值指明出错时，才检验其值。
第二条规则是：任何函数都不会将 *errno* 值设为0，而且在 `<errno.h>` 中定义的所有常量都不为0。**

C标准库定义了两个函数用于打印出错信息：[strerror](http://man7.org/linux/man-pages/man3/strerror.3.html)，[perror](http://man7.org/linux/man-pages/man3/perror.3.html)。

### Figure-1.8.c

```c
#include "apue.h"
#include <errno.h>

/* gcc apue.h apue_err.c figure-1.8.c */
int
main(int argc, char *argv[])
{
    fprintf(stderr, "EACCESS: %s\n", strerror(EACCES));
    errno = ENOENT;
    perror(argv[0]);
    exit(0);
}
```

执行结果：

```sh
[fanbin@localhost apue]$ ./a.out 
EACCESS: Permission denied
./a.out: No such file or directory
[fanbin@localhost apue]$ 
```

**出错恢复**
`errno.h` 中定义的各种错误可分为两类：
致命性：无法执行恢复动作，最多打印一条出错消息。
非致命性：可妥善处理。如网络连接不再起作用，短时间延迟后可重试，重试方法可使用 [指数补偿算法](https://en.wikipedia.org/wiki/Exponential_backoff 'Exponential backoff - wikipedia')（参见 [16.4 建立连接](#164-建立连接) 例子）；运用良好的处理方法可提高应用程序的 [健壮性](https://en.wikipedia.org/wiki/Robustness 'Robustness - wikipedia') 。

[BACK TO TOP](#目录)

## 1.8 用户标识

### Figure-1.9.c

功能：获取当前进程的用户uid、gid
涉及头文件：[unistd.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html)，[sys/types.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_types.h.html)
涉及函数：[getuid](http://pubs.opengroup.org/onlinepubs/9699919799/functions/getuid.html)，[getgid](http://pubs.opengroup.org/onlinepubs/9699919799/functions/getgid.html)

```c
#include "apue.h"

/* gcc apue.h figure-1.9.c */
int
main(void)
{
    printf("uid = %d, gid = %d\n", getuid(), getgid());
    exit(0);
}
```

[BACK TO TOP](#目录)

## 1.9 信号

信号用于通知进程发生了某种情况。进程有如下3中处理信号的方式：
1. 忽略信号。
2. 按系统默认方式处理。
3. 提供一个函数，信号发生时调用函数，捕捉该信号，按预期处理信号。
很多情况都会产生信号，如 [中断键、退出键](#快捷命令)，也可以利用 [kill](http://man7.org/linux/man-pages/man1/kill.1.html) 命令产生。

### Figure-1.10.c

功能：在 [Figure-1.7.c](#figure-17c) 基础上，捕获 `^C` 信号
涉及头文件：[signal.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/signal.h.html)
涉及函数：[signal](http://man7.org/linux/man-pages/man2/signal.2.html)
沉淀内容：了解如何捕获异常信号，了解异常信号枚举值所在头文件 `signal.h`

```c
#include "apue.h"
#include <sys/signal.h>

static void sig_int(int);   /* our signal-catching function */

/* gcc apue.h apue_err.c figure-1.10.c */
int
main(void)
{
    char    buf[MAXLINE];   /* from apue.h */
    pid_t   pid;
    int     status;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% ");  /* print prompt (printf requrires %% to print %) */
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; /* replace newline with null */

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {  /* child */
            execlp(buf, buf, (char *)0);
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");
        printf("%% ");
    }
    exit(0);
}

void
sig_int(int signo)
{
    printf("interrupt\n%% ");
}
```

执行结果：

```sh
[fanbin@localhost apue]$ ./a.out 
% ps
  PID TTY          TIME CMD
  657 pts/22   00:00:00 a.out
  658 pts/22   00:00:00 ps
28202 pts/22   00:00:01 bash
% pwd
/home/fanbin/learn/apue
% ^Cinterrupt
% [fanbin@localhost apue]$ 
```

此函数由 [Figure-1.7.c](#figure-17c)，改造而成。

[BACK TO TOP](#目录)

## 1.11 系统调用和库函数

从实现这的角度来看，系统调用和库函数之间有根本区别，但从用户角度来看，其区别不重要。 **我们应当理解的是，如果希望的话，我们可以替换库函数，但是系统调用通常不能被替换的。** 系统调用和库函数之间的一个差别是： **系统调用通常提供一种最小接口，而库函数通常提供比较复杂的功能。** 从 [brk](http://man7.org/linux/man-pages/man2/brk.2.html) 系统调用、 [sbrk](http://man7.org/linux/man-pages/man2/sbrk.2.html) 系统函数 ( **在Linux中实现为库函数** ) 和 [malloc](http://man7.org/linux/man-pages/man3/malloc.3.html) 库函数调用可以看出差别。

[BACK TO TOP](#目录)

---------------------
第二章 UNIX标准及实现
=====================

**关于各类常量、常用类型、头文件的描述。**

##2.5.4 函数sysconf,pathconf和fpathconf

###Figure-2.14.c

功能：查询特定系统实现支持的限制
涉及头文件：[errno.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html)，[limits.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/limits.h.html)
涉及函数：[sysconf](http://man7.org/linux/man-pages/man3/sysconf.3.html)，[pathconf](http://man7.org/linux/man-pages/man3/pathconf.3.html)
沉淀内容：学会如何查询系统某一限制，及如何扩平台编程， `sysconf -> _SC_XXX`， `pathconf -> _PC_XXX`

```c
#include "apue.h"
#include <errno.h>
#include <limits.h>

static void pr_sysconf(char *, int);
static void pr_pathconf(char *, char *, int);

/* gcc apue.h apue_err.c figure-2.14.c */
int
main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("usage: a.out <dirname>");

#ifdef ARG_MAX
    printf("ARG_MAX defined to be %ld\n", (long)ARG_MAX + 0);
#else
    printf("no symbol for ARG_MAX\n");
#endif
#ifdef _SC_ARG_MAX
    pr_sysconf("ARG_MAX =", _SC_ARG_MAX);
#else
    printf("no symbol for _SC_ARG_MAX\n");
#endif

/* similar processing for all the rest of the sysconf symbols... */

#ifdef MAX_CANON
    printf("MAX_CANON defined to be %ld\n", (long)MAX_CANON + 0);
#else
    printf("no symbol for MAX_CANON\n");
#endif
#ifdef _PC_MAX_CANON
    pr_pathconf("MAX_CANON =", argv[1], _PC_MAX_CANON);
#else
    printf("no symbol for _PC_MAX_CANON\n");
#endif

/* similar processing for all the rest of the pathconf symbols... */

    exit(0);
}

static void
pr_sysconf(char *mesg, int name)
{
    long    val;

    fputs(mesg, stdout);
    errno = 0;
    if ((val = sysconf(name)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL)
                fputs(" (not supported)\n", stdout);
            else
                err_sys("sysconf error");
        } else {
            fputs(" (no limit)\n", stdout);
        }
    } else {
        printf(" %ld\n", val);
    }
}

static void
pr_pathconf(char *mesg, char *path, int name)
{
    long    val;

    fputs(mesg, stdout);
    errno = 0;
    if ((val = pathconf(path, name)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL)
                fputs(" (no supported)\n", stdout);
            else
                err_sys("pathconf error, path = %s", path);
        } else {
            fputs(" (no limit)\n", stdout);
        }
    } else {
        printf(" %ld\n", val);
    }
}
```

执行结果：
```sh
[fanbin@localhost apue]$ gcc apue.h apue_err.c figure-2.14.c 
[fanbin@localhost apue]$ ./a.out .
no symbol for ARG_MAX
ARG_MAX = 2621440
MAX_CANON defined to be 255
MAX_CANON = 255
[fanbin@localhost apue]$ 
```

[BACK TO TOP](#目录)

## 2.5.5 不确定运行时限制

### Figure-2.16.c

功能：为路径名动态分配存储区。
沉淀内容：对于某些不确定的限制值，提供一种解决方案。

```c
#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;

/* If PATH_MAX is determinate, no guarantee this is adequate */
#define PATH_MAX_GUESS 1024

char *
path_alloc(size_t *sizep)   /* also return allocated size, if nonnull */
{
    char    *ptr;
    size_t   size;

    if (posix_version == 0)
        posix_version = sysconf(_SC_VERSION);

    if (xsi_version == 0)
        xsi_version = sysconf(_SC_XOPEN_VERSION);

    if (pathmax == 0) { /* first time through */
        errno = 0;
        if ((pathmax == pathconf("/", _PC_PATH_MAX)) < 0) {
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;   /* it's indeterminate */
            else
                err_sys("pathconf error for _PC_PATH_MAX");
        } else {
            pathmax++;  /* add one since it's relative to root */
        }
    }

    /*
     * Before POSIX.1-2001, we aren't guaranteed that PATH_MAX includes
     * the terminating null byte. Same goes for XPG3.
     */
    if ((posix_version < 200112L) && (xsi_version < 4))
        size = pathmax = 1;
    else
        size = pathmax;

    if ((ptr = malloc(size)) == NULL)
        err_sys("malloc error for pathname");

    if (sizep != NULL)
        *sizep = size;
    return(ptr);
}
```

### Figure-2.17.c

功能：获取进程可打开的最大文件描述符

```c
#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

/*
 * If OPEN_MAX is indeterminate, this might be inadequate.
 */
#define OPEN_MAX_GUESS 256
#endif

long
open_max(void)
{
    if (openmax == 0) {   /* first time throught */
        errno = 0;
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
            if (errno == 0)
                openmax = OPEN_MAX_GUESS;   /* it's indeterminate */
            else
                err_sys("sysconf error for _SC_OPEN_MAX");
        }
    }
    return(openmax);
}
```

## 2.6 选项

**如果我们要编写可移植的应用程序，而这些程序可能依赖于这些可选的支持的功能，那么就需要一种可移植的方法来判断实现是否支持一个给定的选项。**
如同对限制的处理一样（见2.5节）一样，POSIX.1 定义了3种处理选项的方法。
(1) 编译时选项定义在 `<unistd.h>` 中
(2) 与文件或目录无关的运行时选项用 `sysconf` 函数来判断
(3) 与文件或目录有关的运行时选项通过调用 `pathconf` 或 `fpathconf` 函数来判断

[BACK TO TOP](#目录)

## 2.8 基本系统数据类型

在头文件 `<sys/types.h>` 中，这些数据类型都是用 C 的 `typedef` 来定义的，他们绝大多数都是以 `_t` 结尾。

[BACK TO TOP](#目录)

[幻常量、幻数](https://en.wikipedia.org/wiki/Magic_number_(programming) 'Magic number - wikipedia')

[BACK TO TOP](#目录)

--------------
第三章 文件I/O
==============

本章描述的IO函数经常被称为 *不带缓冲的I/O* (unbuffered I/O，与将在 [第五章](#第五章-标准io库) 中说明的 *标准I/O函数* 相对照)。术语 **不带缓冲** 指的是每个 `read` 和 `write` 都调用内核中的一个系统调用。

## 3.6 函数lseek

### Figure-3.1.c

功能：测试对标准输入能否设置偏移量
涉及函数：[lseek](http://man7.org/linux/man-pages/man2/lseek.2.html)
沉淀内容：学会如何使用 `lseek`

```c
#include "apue.h"

/* gcc apue.h figure-3.1.c */
int
main(void)
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        printf("cann't seek\n");
    else
        printf("seek OK\n");
    exit(0);
}
```
执行结果：
```sh
[fanbin@localhost apue]$ ./a.out < /etc/passwd
seek OK
[fanbin@localhost apue]$ cat < /etc/passwd| ./a.out 
cann't seek
[fanbin@localhost apue]$
```
通常，文件的当前偏移量应当是一个非负整数，但是，某些设备也可能允许负的偏移量。但对于普通文件，其偏移量必须是非负值。**因为偏移量可能是负值，所以在比较 `lseek` 的返回值时应当谨慎，不要测试他是否小于0，而要测试它是否等于-1。**

### Figure-3.2.c

功能：创建出一个具有空洞的文件
涉及头文件：[fcntl.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/fcntl.h.html)
涉及函数：[create](http://man7.org/linux/man-pages/man2/creat.2.html)
涉及命令：[od](http://man7.org/linux/man-pages/man1/od.1.html)
沉淀内容：了解 **空洞** 含义，了解如何创建出空洞文件，如何dump文件内容

```c
#include "apue.h"
#include <fcntl.h>

char    buf1[] = "abcdefghij";
char    buf2[] = "ABCDEFGHIJ";

/* gcc apue.h apue_err.c figure-3.2.c */
int
main(void)
{
    int     fd;

    if ((fd = creat("file.hole", FILE_MODE)) < 0)
        err_sys("create error");

    if (write(fd, buf1, 10) != 10)
        err_sys("buf1 write error");
    /* offset now = 10 */

    if (lseek(fd, 16384, SEEK_SET) == -1)
        err_sys("lseek error");
    /* offset now = 16384 */

    if (write(fd, buf2, 10) != 10)
        err_sys("buf2 write error");
    /* offset now = 16394 */

    exit(0);
}
```
执行结果：
```sh
[fanbin@localhost apue]$ ls -l file.hole  # 检查其大小
-rw-r--r--. 1 fanbin fanbin 16394 Feb 26 17:13 file.hole
[fanbin@localhost apue]$ od -c file.hole  # 观察实际内容
0000000   a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
0040000   A   B   C   D   E   F   G   H   I   J
0040012
[fanbin@localhost apue]$ 
```

**文件偏移量可以大于文件的当前长度，在这种情况下，对该文件的下一次写将加长该文件，并在文件中构成一个 [空洞（spare file）](https://en.wikipedia.org/wiki/Sparse_file 'Spare file - wikipedia') ，这一点是允许的。** 位于文件中但没有写过的字节都被读为0。

**文件中的空洞并不要求在磁盘上占用存储区。** 具体处理方式与文件系统的实现相关，当定位到超出文件尾端之后写时，对于新写的数据需要分配磁盘块，但是对于原文件尾端和新开始写位置之间的部分则不需要分配磁盘块。

[4.12节更多详细内容](#xxx)

[BACK TO TOP](#目录)

## 3.9 I/O的效率

### Figure-3.5.c

```c
#include "apue.h"

#define BUFFSIZE    4096

/* gcc apue.h apue_err.c figure-3.5.c */
int
main(void)
{
    int     n;
    char    buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");

    if (n < 0)
        err_sys("read error");

    exit(0);
}
```

此节需看书上内容，缓冲区的大小通常为磁盘块长度 4096（磁盘块长度有 `st_blksize` 表示，在 [4.12小结](#xx) 中说明其值为 4096）

[BACK TO TOP](#目录)

## 3.11 原子操作

UNIX系统提供一种原子操作方法，即在打开文件时设置 [O_APPEND](http://man7.org/linux/man-pages/man2/open.2.html) 标志，使得内核在每次写操作之前，都将进程的当前偏移量设置到该文件的尾端处，于是在每次写之前就不再需要调用 `lseek`。

扩展知识参见 [4.15](#xx)，[14.3](#xx)

[BACK TO TOP](#目录)

## 3.14 函数fcntl

**函数 `fcntl` 可以改变已经打开文件的属性。** 具有以下5中功能：
1. 复制一个已有的描述符（F_DUPFD, F_DUPFD_CLOEXEC）
2. 获取/设置文件描述符标识（F_GETFD，F_SETFD）
3. 获取/设置文件状态标识（F_GETFL，F_SETFL）， 例 [Figure-3.12.c](#figure-312c)
4. 获取/设置异步I/O所有权（F_GETOWN，F_SETOWN）
5. 获取/设置记录锁（F_GETLK，F_SETLK，F_SETLKW）

### Figure-3.11.c

功能：对指定的文件描述符，打印其所选的文件的标志说明。
涉及头文件：[fcntl.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/fcntl.h.html)，[stdlib.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdlib.h.html)
涉及函数：[fcntl](http://man7.org/linux/man-pages/man2/fcntl.2.html)，[atoi](http://man7.org/linux/man-pages/man3/atoi.3.html)
沉淀内容：如何利用 `fcntl` 获取文件的标志说明

```c
#include "apue.h"
#include <fcntl.h>

/* gcc apue.h apue_err.c figure-3.11.c */
int
main(int argc, char *argv[])
{
    int     val;
    
    if (argc != 2)
        err_quit("usage: a.out <descriptor#>");

    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
        err_sys("fcntl error for fd %d", atoi(argv[1]));

    switch (val & O_ACCMODE) {
    case O_RDONLY:
        printf("read only");
        break;

    case O_WRONLY:
        printf("write only");
        break;
        
    case O_RDWR:
        printf("read write");
        break;

    default:
        err_dump("unknown access mode");
    }

    if (val & O_APPEND)
        printf(", append");
    if (val & O_NONBLOCK)
        printf(", nonblocking");
    if (val & O_SYNC)
        printf(", synchronous writes");

#if !defined(_POSIX_C_SOURCE) && defined(O_FYNC) && (O_FSYNC != O_SYNC)
    if (val & O_FSYNC)
        printf(", synchronous writes");
#endif

    putchar('\n');
    exit(0);
}
```

执行结果：

```sh
[fanbin@localhost apue]$ ./a.out 0 < /dev/tty
read only
[fanbin@localhost apue]$ ./a.out 1 > temp.foo
[fanbin@localhost apue]$ cat temp.foo 
write only
[fanbin@localhost apue]$ ./a.out 2 2>>temp.foo
write only, append
[fanbin@localhost apue]$ ./a.out 5 5<>temp.foo
read write
[fanbin@localhost apue]$ 
```

`5<>temp.foo` 表示在文件描述符5上打开文件来读写。
 
### Figure-3.12.c

功能：清除/设置指定文件标志
沉淀内容：学会利用简单的位运算符 开启/关闭 某项/多项 功能

```c
#include "apue.h"
#include <fcntl.h>

void
set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int     val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    val |= flags;   /* turn on flags */

    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}

void
clr_fl(int fd, int flags) /* flags turn off */
{
    int     val;
    
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    val &= ~flags;  /* turn flags off */

    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}
```

若在 [Figure-3.5.c](#figure-35c) 的开始处加上下面一行以调用 `set_fl`，则开启了同步写标志。

    set_fl(STDOUT_FILENO, O_SYNC)
这就使每次 `write` 都要等待，直至数据已写到磁盘上再返回。在UNIX系统中，通常 `write` 只是将数据排入队列，而实际的磁盘操作则可能在以后的某个时刻进行。**而数据库系统则需要使用 [O_SYNC](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/fcntl.h.html)，这样一来，当它从 `write` 返回时就知道数据已经确实写到了磁盘上，以免在系统异常时产生数据丢失。**

[BACK TO TOP](#目录)

## 3.17 小结

[BACK TO TOP](#目录)

-----------------
第四章 文件和目录
=================

## 4.2 函数stat，fstat，fstatat和lstat

[stat](http://man7.org/linux/man-pages/man2/stat.2.html)：返回与命名文件有关的信息结构
[fstat](http://man7.org/linux/man-pages/man2/fstat.2.html)：获得已在描述符上打开的文件的有关信息
[lstat](http://man7.org/linux/man-pages/man2/lstat.2.html)：当命名的文件时一个符号链接时，返回该符号链接有关信息
[fstatat](http://man7.org/linux/man-pages/man2/fstatat.2.html)：为一个相对于当前打开目录的路径名返回文件统计信息

使用 `stat` 函数最多的地方可能就是 `ls -l` 命令，用其可以获得有关一个文件的所有信息。

[BACK TO TOP](#目录)

## 4.3 文件类型

（1）普通文件（regular file）
（2）目录文件（directory file）
（3）块特殊文件（block special file）
（4）字符特殊文件（character special file）
（5）FIFO
（6）套接字（socket）
（7）符号链接（symbolic link）

### Figure-4.3.c

功能：查看文件/目录的文件类型
涉及头文件：[sys/stat.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_stat.h.html)
涉及函数：[lstat](http://man7.org/linux/man-pages/man2/lstat.2.html)
沉淀内容：了解命令 `ls -l` 的操作函数及如何获取文件信息

```c
#include "apue.h"

/* gcc apue.h apue_err.c figure-4.1.c */
int
main(int argc, char *argv[])
{
    int          i;
    struct stat  buf;
    char        *ptr;

    for (i = 1; i < argc; ++i) {
        printf("%s: ", argv[i]);
        if (lstat(argv[i], &buf) < 0) {
            err_ret("lstat error");
            continue;
        }
        if (S_ISREG(buf.st_mode))
            ptr = "regular";
        else if (S_ISDIR(buf.st_mode))
            ptr = "directory";
        else if (S_ISCHR(buf.st_mode))
            ptr = "directory";
        else if (S_ISBLK(buf.st_mode))
            ptr = "character special";
        else if (S_ISFIFO(buf.st_mode))
            ptr = "fifo";
        else if (S_ISLNK(buf.st_mode))
            ptr = "symbolic link";
        else if (S_ISSOCK(buf.st_mode))
            ptr = "socket";
        else
            ptr = "** unknown mode **";
        printf("%s\n", ptr);
    }
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out /etc/passwd /etc /dev/log /dev/sr0 /dev/cdrom 
/etc/passwd: regular
/etc: directory
/dev/log: socket
/dev/sr0: character special
/dev/cdrom: symbolic link
[fanbin@localhost apue]$ 
```

利用 `lstat` 查看某一文件/目录的 `struct stat` 结构：
```c
#include "apue.h"

#define print(x) printf("%-15s: %d\n", #x, x)

int
main(int argc, char *argv[])
{
    struct stat     buf;
    if (lstat(argv[1], &buf) < 0) {
        err_ret("lstat error");
    }
    /* man lstat */
    print(buf.st_dev);      /* ID of device containing file */
    print(buf.st_ino);      /* inode number */
    print(buf.st_mode);     /* protection */
    print(buf.st_nlink);    /* number of hard links */
    print(buf.st_uid);      /* user ID of owner */
    print(buf.st_gid);      /* group ID of owner */
    print(buf.st_rdev);     /* device ID (if special file) */
    print(buf.st_size);     /* total size, in bytes */
    print(buf.st_blksize);  /* blocksize for file system I/O */
    print(buf.st_blocks);   /* number of 512B blocks allocated */
    print(buf.st_atime);    /* time of last access */
    print(buf.st_mtime);    /* time of last modification */
    print(buf.st_ctime);    /* time of last status change */
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out .
buf.st_dev     : 64768
buf.st_ino     : 266778
buf.st_mode    : 16893
buf.st_nlink   : 18
buf.st_uid     : 500
buf.st_gid     : 500
buf.st_rdev    : 0
buf.st_size    : 4096
buf.st_blksize : 4096
buf.st_blocks  : 8
buf.st_atime   : 1456737027
buf.st_mtime   : 1456737018
buf.st_ctime   : 1456737018
[fanbin@localhost apue]$ 
```

[BACK TO TOP](#目录)

## 4.5 文件访问权限
先摘录APUE中关于内核对访问文件的测试步骤：

> 进程每次打开、创建或删除一个文件时，内核就进行文件访问权限测试，而这种测试可能涉及文件的所有者（st_uid 和 st_gid）、进程的有效ID（有效用户ID和有效组ID）以及进程的附属组ID（若支持的话）。两个所有者ID是文件的性质，而两个有效ID和附属ID则是进程的性质。内核进行的测试具体如下。

> （1）若进程的有效用户ID是0（超级用户），则允许访问。

> （2）若进程的有效用户ID等于文件的所有者ID（也就是进程拥有此文件），那么如果所有者适当的访问权限位被设置，则允许访问；否则拒绝访问。

> （3）如进程的有效组ID或进程的附属组ID之一等于文件的组ID，那么如果组适当的访问权限位被设置，则允许访问；否则拒绝访问。

> （4) 若其他用户适当的访问权限位被设置，则允许访问；否则拒绝访问。

> 按顺序执行以上4步。

[最小特权模型（最小权限原则，Principle of least privilege）](https://en.wikipedia.org/wiki/Principle_of_least_privilege 'Principle of least privilege - wikipedia')

当我们在权限不足的情况下需要执行特权操作时，需要指定文件设置了有效用户ID；若文件拥有有效用户ID，当我们执行操作时，有效用户ID会临时替换为文件的所有者ID并执行，执行完成后进程切换回实际用户ID，避免长时间拥有特权命令。

在 [Figure-4.7.c](#figure-47c) 的执行结果中 `-rwsrwxr-x. 1 root fanbin 6923 3月   1 10:03 a.out` 字母 `s` 指示出该文件拥有设置用户ID权限。

其他参考资料：
http://blog.csdn.net/demiaowu/article/details/39370355
http://blog.chinaunix.net/uid-26817066-id-3275353.html

[BACK TO TOP](#目录)

## 4.7 函数access和faccessat

### Figure-4.8.c
功能：用实际用户ID/有效用户ID访问文件
涉及头文件：[fcntl.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/fcntl.h.html)，[unistd.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html)
涉及函数：[open](http://man7.org/linux/man-pages/man2/open.2.html)，[access](http://man7.org/linux/man-pages/man2/access.2.html)

```c
#include "apue.h"
#include <fcntl.h>

/* gcc apue.h apue_err.c figure-4.8.c */
int
main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("usage: a.out <pathname>");
    if (access(argv[1], R_OK) < 0)
        err_ret("access error for %s", argv[1]);
    else
        printf("read access OK\n");
    if (open(argv[1], O_RDONLY) < 0)
        err_ret("open error for %s", argv[1]);
    else
        printf("open for reading OK\n");
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ls -l a.out
-rwxrwxr-x. 1 fanbin fanbin 6923 Mar  1 10:03 a.out
[fanbin@localhost apue]$ ./a.out a.out
read access OK
open for reading OK
[fanbin@localhost apue]$ ls -l /etc/shadow
----------. 1 root root 945 Sep 21 19:16 /etc/shadow
[fanbin@localhost apue]$ ./a.out /etc/shadow
access error for /etc/shadow: Permission denied
open error for /etc/shadow: Permission denied
[fanbin@localhost apue]$ su              # 成为超级用户
Password: 
[root@localhost apue]# chown root a.out  # 将文件用户ID改为root
[root@localhost apue]# chmod u+s a.out   # 并打开设置用户ID位
[root@localhost apue]# ls -l a.out       # 检查所有者和SUID位
-rwsrwxr-x. 1 root fanbin 6923 3月   1 10:03 a.out
[root@localhost apue]# exit              # 恢复为正常用户
exit
[fanbin@localhost apue]$ ./a.out /etc/shadow
access error for /etc/shadow: Permission denied
open for reading OK
[fanbin@localhost apue]$ 
```

由本节内容结合4.5节内容可知，当用 `open` 函数打开一个文件时，内核以进程的有效用户ID和有效组ID为基础执行其访问权限测试，当用 `access` 和 `faccessat` 函数时，是按实际用户ID和实际组ID进行访问权限测试。

## 4.8 函数umask

### Figure-4.9.c
功能：创建指定文件模式的文件
涉及头文件：[sys/stat.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_stat.h.html)
涉及函数：[umask](http://man7.org/linux/man-pages/man2/umask.2.html)
沉淀内容：如何利用 `umask` 创建所需文件模式/权限的文件

```c
#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

/* gcc apue.h apue_err.c figure-4.8.c */
int
main(void)
{
    umask(0);
    if (creat("foo", RWRWRW) < 0)
        err_sys("creat error for foo");
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); /* 屏蔽组用户、其他用户的读写权限 */
    if (creat("bar", RWRWRW) < 0)
        err_sys("creat error for bar");
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ umask # 先打印当前文件模式创建屏蔽字
0002
[fanbin@localhost apue]$ ./a.out 
[fanbin@localhost apue]$ ls -l foo bar
-rw-------. 1 fanbin fanbin 0 Mar  1 10:45 bar
-rw-rw-rw-. 1 fanbin fanbin 0 Mar  1 10:45 foo
[fanbin@localhost apue]$ umask # 观察文件模式创建屏蔽字是否更改
0002
[fanbin@localhost apue]$ 
```

当编写创建新文件的程序时，如果我们想确保指定的访问权限位已经激活，那么必须在进程运行时修改 `umask` 值。

[BACK TO TOP](#目录)

## 4.9 函数chmod，fchmod和fchmodat

### Figure-4.12.c
功能：更改文件的指定权限
涉及头文件：[sys/stat.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_stat.h.html)
涉及函数：[chmod](http://man7.org/linux/man-pages/man2/chmod.2.html)
沉淀内容：理解并学会使用 `chmod` 相关函数更改指定文件权限

```c
#include "apue.h"

/* gcc apue.h apue_err.c figure-4.12.c */
int
main(void)
{
    struct stat     statbuf;

    /* turn on set-group-ID and turn off group-execute */

    if (stat("foo", &statbuf) < 0)
        err_sys("stat error for foo");
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        err_sys("chmod error for foo");

    /* set absolute mode to "rw-r--r--" */
    
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
        err_sys("chmod error for bar");
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ls -l foo bar         # 先查看由figure-4.9.c程序执行结果
-rw-------. 1 fanbin fanbin 0 Mar  1 10:45 bar
-rw-rw-rw-. 1 fanbin fanbin 0 Mar  1 10:45 foo
[fanbin@localhost apue]$ ./a.out               # 更改文件权限
[fanbin@localhost apue]$ ls -l foo bar
-rw-r--r--. 1 fanbin fanbin 0 Mar  1 10:45 bar
-rw-rwSrw-. 1 fanbin fanbin 0 Mar  1 10:45 foo
[fanbin@localhost apue]$ 
```

**`ls` 命令将组执行权限表示为 `S`，它表示设置组ID为已经打开，同时，组执行位未设置。** 在程序运行之后，`ls` 命令列出的时间和日期并没有改变。在 [4.19](##) 节中，我们会了解到 `chmod` 函数更新的只是 `i` 节点最近一次被更改的时间。按系统默认方式，`ls -l` 列出的是最后修改文件内容的时间。

## 4.14 文件系统
在原文中有两个目录项指向同一个 i 节点。每个 i 节点都有一个链接计数，其值是指向该 i 节点的目录项数。只有当链接计数减少至 0 时，才可删除该文件（也就是说可以释放该文件占用的数据块）。这就是为什么“接触对一个文件的链接”操作并不总是意味着“释放改文件占用的磁盘块”的原因。**这也是为什么删除一个目录项的函数被称之为 `unlink` 而不是 `delete` 的原因。**在 [stat](#figure-43c) 结构中，连接技术包含在 `st_nlink` 成员中，其基本系统数据类型是 `nlink_t`。这种链接类型称为 [硬链接](https://en.wikipedia.org/wiki/Hard_link 'Hard link - wikipedia')

摘录 [百度百科](http://baike.baidu.com/view/4328569.htm) 关于软链接、硬链接的描述

> 软链接可以跨文件系统，硬链接不可以；软链接可以对一个不存在的文件名(filename)进行链接（当然此时如果你vi这个软链接文件，linux会自动新建一个文件名为filename的文件）,硬链接不可以（其文件必须存在，inode必须存在）；软链接可以对目录进行连接，硬链接不可以。两种链接都可以通过命令 ln 来创建。ln 默认创建的是硬链接。使用 -s 开关可以创建软链接。

或看APUE [4.17 符号链接](##) 章节。

[BACK TO TOP](#目录)

## 4.15 函数link、linkat、unlink、unlinkat和remove

### Figure-4.16.c
功能：打开一个文件，解除他的链接，休眠15秒后，程序终止。
涉及头文件：[fcntl.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/fcntl.h.html)
涉及函数：[unlink](http://man7.org/linux/man-pages/man2/unlink.2.html)
沉淀内容：了解如何删除进程中创建的临时文件。

```c
#include "apue.h"
#include <fcntl.h>

/* gcc apue.h apue_err.c figure-4.16.c */
int
main(void)
{
    if (open("tempfile", O_RDWR) < 0)
        err_sys("open error");
    if (unlink("tempfile") < 0)
        err_sys("unlink error");
    printf("file unlinked.\n");
    sleep(15);
    printf("done\n");
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ls -l tempfile    # 查看文件大小
-rw-rw-r--. 1 fanbin fanbin 1484720 Mar  1 15:34 tempfile
[fanbin@localhost apue]$ df /home          # 检查可用磁盘空间
Filesystem           1K-blocks    Used Available Use% Mounted on
/dev/mapper/vg_centos-lv_root
                      17969940 8274780   9511332  47% /
[fanbin@localhost apue]$ ./a.out &         # 在后台运行程序
[1] 7069
[fanbin@localhost apue]$ file unlinked.
ls -l tempfile                             # 观察文件是否仍然存在
ls: cannot access tempfile: No such file or directory
[fanbin@localhost apue]$ df /home          # 检查可用磁盘空间有无变化
Filesystem           1K-blocks    Used Available Use% Mounted on
/dev/mapper/vg_centos-lv_root
                      17969940 8274780   9511332  47% /
[fanbin@localhost apue]$ done
df /home                                   # 现在，应当有更多的可用磁盘空间
Filesystem           1K-blocks    Used Available Use% Mounted on
/dev/mapper/vg_centos-lv_root
                      17969940 8273320   9512792  47% /
[1]+  Done                    ./a.out
[fanbin@localhost apue]$ 
```

**`unlink` 的这种特性经常被程序用来确保即使是在程序崩溃时，它所创建的临时文件也不会遗留下来。**

进程用 [open](http://man7.org/linux/man-pages/man2/open.2.html) 或 [creat](http://man7.org/linux/man-pages/man2/creat.2.html) 创建一个文件，然后立即调用unlink，因为该文件仍旧是打开的，所以不会将其内容删除。只有当进程关闭该文件或终止时（在这种情况下，内核关闭该进程所打开的全部文件），该文件的内容才被删除。

我们也可以用 [remove](http://man7.org/linux/man-pages/man3/remove.3.html) 函数解除对一个文件或目录的链接。对于文件，[remove](http://man7.org/linux/man-pages/man3/remove.3.html) 的功能与 `unlink` 相同。对于目录，[remove](http://man7.org/linux/man-pages/man3/remove.3.html) 的功能与 [rmdir](http://man7.org/linux/man-pages/man2/rmdir.2.html) 相同。

[BACK TO TOP](#目录)

## 4.19 文件的时间

## 4.20 函数futimens、utimensat和utimes

### Figure-4.21.c
功能：使用带 *O_TRUNC* 选项的 `open` 函数将文件长度截断为 0，但并不更改其访问时间及修改时间。
涉及头文件：[sys/stat.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_stat.h.html)
涉及函数：[futimens](http://man7.org/linux/man-pages/man3/futimens.3.html)，[stat](http://man7.org/linux/man-pages/man2/stat.2.html)
沉淀内容：如何利用API更改文件的 `mtime, atime, ctime`，本地 `man` 内容过旧后，如何查询在线 `man` 以获取所需文档，学会利用 `uname -a` 命令查看本机内核版本。

```c
#include "apue.h"
#include <fcntl.h>

/* gcc apue.h apue_err.c figure-4.21.c */
int
main(int argc, char *argv[])
{
    int             i, fd;
    struct stat     statbuf;
    struct timespec times[2];
    for (i = 1; i < argc; ++i) {
        if (stat(argv[i], &statbuf) < 0) {  /* fetch current time */
            err_ret("%s: stat error", argv[i]);
            continue;
        }
        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {  /* truncate */
            err_ret("%s: open error", argv[i]);
            continue;
        }
        times[0] = statbuf.st_atim;
        times[1] = statbuf.st_mtim;
        if (futimens(fd, times) < 0)    /* reset times */
            err_ret("%s: futimens error", argv[i]);
        close(fd);
    }
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ls -l changemod times       # 查看长度和最后修改时间
-rw-rw-r--. 1 fanbin fanbin 2449 Mar  1 17:06 changemod
-rw-rw-r--. 1 fanbin fanbin 2415 Mar  1 17:06 times
[fanbin@localhost apue]$ ls -lu changemod times      # 查看最后访问时间
-rw-rw-r--. 1 fanbin fanbin 2449 Mar  1 17:05 changemod
-rw-rw-r--. 1 fanbin fanbin 2415 Mar  1 17:05 times
[fanbin@localhost apue]$ date                        # 打印当天日期
Tue Mar  1 18:05:48 CST 2016
[fanbin@localhost apue]$ ./a.out changemod times     # 运行程序
[fanbin@localhost apue]$ ls -l changemod times       # 检查结果
-rw-rw-r--. 1 fanbin fanbin 0 Mar  1 17:06 changemod
-rw-rw-r--. 1 fanbin fanbin 0 Mar  1 17:06 times
[fanbin@localhost apue]$ ls -lu changemod times      # 检查最后访问时间
-rw-rw-r--. 1 fanbin fanbin 0 Mar  1 17:05 changemod
-rw-rw-r--. 1 fanbin fanbin 0 Mar  1 17:05 times
[fanbin@localhost apue]$ ls -lc changemod times      # 检查状态更改时间
-rw-rw-r--. 1 fanbin fanbin 0 Mar  1 18:06 changemod
-rw-rw-r--. 1 fanbin fanbin 0 Mar  1 18:06 times
[fanbin@localhost apue]$ 
```

在本机 `man 2 stat` 中对应的 `struct stat` 结构文档过旧，查询在线 `man` 后获取新文档说明，说明中指示了新结构在 kernel 2.6 之后支持新结构，利用命令 `uname -a` 查询本机系统内核版本，以了解本机内核是否支持新结构。

[BACK TO TOP](#目录)

## 4.22 读目录

### Figure-4.22.c
功能：做一个简易的遍历文件层次结构的程序( `ftw, nftw` )
涉及头文件：[dirent.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/dirent.h.html)，[limits.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/limits.h.html)
涉及函数：[opendir](http://man7.org/linux/man-pages/man3/opendir.3.html)，[readdir](http://man7.org/linux/man-pages/man2/readdir.2.html)，[closedir](http://man7.org/linux/man-pages/man3/closedir.3.html)
涉及的额外函数：[Figure-2.16.c](#figure-216c)
沉淀内容：学会如何处理编译错误，**了解如何遍历目录**。

```c
#include "apue.h"
#include <dirent.h>
#include <limits.h>

/* function type that is called for each filename */
typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc   myfunc;
static int      myftw(char *, Myfunc *);
static int      dopath(Myfunc *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

/* gcc apue.h apue_err.c figure-4.22.c figure-2.16.c -o ftw */
int
main(int argc, char *argv[])
{
    int     ret;

    if (argc != 2)
        err_quit("usage: ftw <starting-pathname>");
    ret = myftw(argv[1], myfunc);   /* does it all */
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1;   /* avoid divide by 0; print 0 for all counts */
    printf("regular files = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories   = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special  = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs         = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("suymbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets       = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);
    exit(ret);
}
/*
 * Descend through the hierarchy, starting at "pathname".
 * The caller's func() is called for every file.
 */
#define FTW_F       1   /* file other than directory */
#define FTW_D       2   /* directory */
#define FTW_DNR     3   /* directory that can't be read */
#define FTW_NS      4   /* file that we can't stat */

static char *fullpath;  /* contains full pathname for every file */
static size_t pathlen;

static int      /* we return whatever func() returns */
myftw(char *pathname, Myfunc *func)
{
    fullpath = path_alloc(&pathlen);    /* malloc PATH_MAX + 1 bytes */
                                        /* ({Figure 2.16}) */
    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc failed");
    }
    strcpy(fullpath, pathname);
    return (dopath(func));
}

/*
 * Descend through the hierarchy, starting at "fullpath".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * recursively for each name in the directory.
 */
static int      /* we return whatever func() returns */
dopath(Myfunc *func)
{
    struct stat         statbuf;
    struct dirent      *dirp;
    DIR                *dp;
    int                 ret, n;

    if (lstat(fullpath, &statbuf) < 0) /* stat error */
        return(func(fullpath, &statbuf, FTW_NS));
    if (S_ISDIR(statbuf.st_mode) == 0) /* not a directory */
        return(func(fullpath, &statbuf, FTW_F));
    /*
     * It's directory. First call func() for the directory,
     * then process each filename in the directory.
     */
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return ret;
    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {   /* expand path buffer */
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc failed");
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;
    if ((dp = opendir(fullpath)) == NULL)   /* can't read directory */
        return(func(fullpath, &statbuf, FTW_DNR));
    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
            continue;   /* ignore dot and dot-dot */
        strcpy(&fullpath[n], dirp->d_name); /* append name after "/" */
        if ((ret = dopath(func)) != 0)  /* recursive */
            break;  /* time to leave */
    }
    fullpath[n - 1] = 0;    /* earse everything from slash onward */
    if (closedir(dp) < 0)
        err_ret("can't close directory %s", fullpath);
    return(ret);
}
static int
myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch (type) {
    case FTW_F:
        switch (statptr->st_mode & S_IFMT) {
        case S_IFREG: nreg++; break;
        case S_IFBLK: nblk++; break;
        case S_IFCHR: nchr++; break;
        case S_IFIFO: nfifo++; break;
        case S_IFLNK: nslink++; break;
        case S_IFSOCK: nsock++; break;
        case S_IFDIR: /* directories should have type = FTW_D */
            err_dump("for S_IFDIR for %s", pathname);
        }
        break;
    case FTW_D:
        ndir++;
        break;
    case FTW_DNR:
        err_ret("can't read directory %s", pathname);
        break;
    case FTW_NS:
        err_ret("stat error for %s", pathname);
        break;
    default:
        err_dump("unknown type %d for pathname %s", type, pathname);
    }
    return(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./ftw /home/fanbin/learn/apue/
regular files   =     153, 89.47 %
directories     =      18, 10.53 %
block special   =       0,  0.00 %
char special    =       0,  0.00 %
FIFOs           =       0,  0.00 %
suymbolic links =       0,  0.00 %
sockets         =       0,  0.00 %
[fanbin@localhost apue]$ 
```

#### **.GCH**
遇到的问题：
```sh
[fanbin@localhost apue]$ gcc apue.h apue_err.c figure-2.16.c figure-4.22.c  -o ftw
figure-2.16.c:18: error: conflicting types for ‘path_alloc’
apue.h:54: note: previous declaration of ‘path_alloc’ was here
```

一开始并没有看明白错误，GOOGLE了一下，参考 [知足常乐](http://blog.sina.com.cn/s/blog_5420e000010185o2.html) 的博客，
> 原因一：原来是因为没有先做函数声明，而函数位于main()之后，在main函数前声明了函数原型后，一切ok.
>  
> 原因二：头文件的被循环引用，在引用时考虑清楚包含顺序
>  
> 原因三： 头文件声明和定义参数稍有不同。例：
> 头文件中声明 `void Hanlder(const char *buf);`
> 在定义时写作 `void Hanlder(char *buf);`
> 这是就会发生 **conflicting types for** 错误问题


第一第二个原因查看了一下并没有问题，因此我们的错误可能在我们使用的 [apue.h](#apueh) 中，查看了一下头文件中关于 `path_alloc` 的声明，发现与对应的函数定义不符，将代码修改为
```c
//char    *path_alloc(int *);                                 /* Figure 2.16 */
char    *path_alloc(size_t *);                                /* Figure 2.16 */
```
再次编译代码，发现仍然报了同样的错误，有些不解，再次GOOGLE，无果。习惯性的 `ls` 了一下当前目录，突然关注到了 `apue.h.gch` 这个文件，在之前的的练习中虽然见过，但没有深究，现在我们来看一看这个文件，先从 `.gch` 后缀扩展名来查询资料，由 [维基百科词条](https://en.wikipedia.org/wiki/GCH) 可知，[.gch](https://en.wikipedia.org/wiki/Precompiled_header 'Precompiled header - wikipedia')是一个预编译的一个头文件，此文件用于减少编译时间，此文件有时会包含已经过时/过旧的数据，导致编译时实际的 `.h` 的修改并没有生效，需删除 `.gch` 文件后重新编译才能获得最新的编译结果。

### nftw.c
上文中实现了一个简易的 [ftw, nftw](http://man7.org/linux/man-pages/man3/ftw.3.html) 函数，接下来我们用实际的 `ftw, nftw` 库函数统计一下结果：

```c
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

static int
calculate_file(const char *fpath, const struct stat *sb,
        int tflag, struct FTW *ftwbuf)
{
    switch (tflag) {
    case FTW_F:
        switch (sb->st_mode & S_IFMT) {
        case S_IFREG: nreg++; break;
        case S_IFBLK: nblk++; break;
        case S_IFCHR: nchr++; break;
        case S_IFLNK: nslink++; break;
        case S_IFSOCK: nsock++; break;
        case S_IFDIR:   /* directories should have type = FTW_D */
            printf("for S_IFDIR for %s", fpath);
        }
        break;
    case FTW_D:
        ndir++;
        break;
    case FTW_DNR:
        printf("can't read directory %s", fpath);
        break;
    case FTW_NS:
        printf("stat error for %s", fpath);
    default:
        printf("unknown type %d for pathname %s", tflag, fpath);
    }
    return(0);
}

int
main(int argc, char *argv[])
{
    int flags = 0;

    if (nftw((argc < 2) ? "." : argv[1], calculate_file, 20, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0) ntot = 1;
    printf("regular files   = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories     = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special   = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special    = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs           = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("suymbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets         = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);
    exit(EXIT_SUCCESS);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out /home/fanbin/learn/apue/
regular files   =     155, 89.60 %
directories     =      18, 10.40 %
block special   =       0,  0.00 %
char special    =       0,  0.00 %
FIFOs           =       0,  0.00 %
suymbolic links =       0,  0.00 %
sockets         =       0,  0.00 %
[fanbin@localhost apue]$ 
```

额外参考资料：
http://blog.sina.com.cn/s/blog_5420e000010185o2.html
https://en.wikipedia.org/wiki/GCH
http://stackoverflow.com/questions/1241399/what-is-a-h-gch-file

[BACK TO TOP](#目录)

## 4.23 函数chdir，fchdir和getcwd

### Figure-4.23.c
功能：切换目录
涉及头文件：[unistd.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html)
涉及函数：[chdir](http://man7.org/linux/man-pages/man2/chdir.2.html)
沉淀内容：因为当前工作目录是进程的一个属性，所以它只影响调用 `chdir` 的进程本身，而不影响其他进程（更详细说明将在 [第八章](#xx) 讲解）。

```c
#include "apue.h"

/* gcc apue.h apue_err.c figure-4.23 */
int
main(void)
{
    if (chdir("/tmp") < 0)
        err_sys("chdir failed");
    printf("chdir to /tmp succeeded\n");
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out 
chdir to /tmp succeeded
[fanbin@localhost apue]$ pwd
/home/fanbin/learn/apue
[fanbin@localhost apue]$ 
```

[BACK TO TOP](#目录)

### Figure-4.24.c
功能：打印工作目录，若有软链则实际工作目录跟随软链
涉及头文件：[unistd.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html)
涉及函数：[chdir](http://man7.org/linux/man-pages/man2/chdir.2.html)，[getcwd](http://man7.org/linux/man-pages/man2/getcwd.2.html)
沉淀内容：学会建立软链，了解 `chdir` 会跟随软链至实际目录，`getcwd` 会返回实际文件目录
涉及额外函数：[Figure-2.16.c](#figure-216c)

前置环境准备：
```sh
[fanbin@localhost apue]$ cd /usr/                               # 切换至/usr/
[fanbin@localhost usr]$ sudo ln -s ../var/spool /usr/spool      # 构建软链
[fanbin@localhost usr]$ cd -                                    # 切换回当前目录
/home/fanbin/learn/apue
[fanbin@localhost apue]$ 
```

源程序：
```c
#include "apue.h"

/* gcc apue.h apue_er.c figure-4.24.c figure-2.16.c */
int
main(void)
{
    char    *ptr;
    size_t   size;

    if (chdir("/usr/spool/cron") < 0)
        err_sys("chdir failed");
    ptr = path_alloc(&size);    /* {(Figure-2.16.c)} */
    if (getcwd(ptr, size) == NULL)
        err_sys("getcwd failed");
    printf("cwd = %s\n", ptr);
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ sudo ./a.out 
[sudo] password for fanbin: 
cwd = /var/spool/cron
[fanbin@localhost apue]$ 
```

## 4.24 设备特殊文件

### Figure-4.25
功能：对每个文件打印出设备号，若文件为字符特殊文件或块特殊文件则还打印st_rdev值
涉及头文件：`/usr/include/sys/sysmacros.h`
涉及函数：[major](http://man7.org/linux/man-pages/man3/major.3.html)，[minor](http://man7.org/linux/man-pages/man3/minor.3.html)
沉淀内容：了解如何查看文件的设备号，如何解决编译错误，基本头文件所在目录

```c
#include "apue.h"
#ifdef SOLARIS
#include <sys/mkdev.h>
#endif
#include <sys/sysmacros.h>      /* 这是个坑 */

/* gcc apue.h apue_err.c figure-4.25.c */
int
main(int argc, char *argv[])
{
    int         i;
    struct stat buf;

    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (stat(argv[i], &buf) < 0) {
            err_ret("stat error");
            continue;
        }
        printf("dev = %d/%d", major(buf.st_dev), minor(buf.st_dev));
        if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
            printf(" (%s) rdev = %d/%d", 
                   (S_ISCHR(buf.st_mode)) ? "character" : "block",
                   major(buf.st_rdev), minor(buf.st_rdev));
        }
        printf("\n");
    }
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out /home/fanbin /dev/tty[01]
/home/fanbin: dev = 253/0
/dev/tty0: dev = 0/5 (character) rdev = 4/0
/dev/tty1: dev = 0/5 (character) rdev = 4/1
[fanbin@localhost apue]$ mount                              # 哪些目录安装在哪些设备上
/dev/mapper/vg_centos-lv_root on / type ext4 (rw)
proc on /proc type proc (rw)
sysfs on /sys type sysfs (rw)
devpts on /dev/pts type devpts (rw,gid=5,mode=620)
tmpfs on /dev/shm type tmpfs (rw,rootcontext="system_u:object_r:tmpfs_t:s0")
/dev/sda1 on /boot type ext4 (rw)
none on /proc/sys/fs/binfmt_misc type binfmt_misc (rw)
.host:/ on /mnt/hgfs type vmhgfs (rw,ttl=1)
vmware-vmblock on /var/run/vmblock-fuse type fuse.vmware-vmblock (rw,nosuid,nodev,default_permissions,allow_other)
[fanbin@localhost apue]$ ls -l /dev/tty[01] /dev/sda[12]
brw-rw----. 1 root disk 8, 1 Sep 21 17:01 /dev/sda1
brw-rw----. 1 root disk 8, 2 Sep 21 17:01 /dev/sda2
crw--w----. 1 root tty  4, 0 Sep 21 17:01 /dev/tty0
crw--w----. 1 root tty  4, 1 Dec 30 10:44 /dev/tty1
[fanbin@localhost apue]$ 
```

若按照原书所示样例，在我所用的环境下会编译失败：
```c
[fanbin@localhost apue]$ gcc apue.h apue_err.c figure-4.25.c 
/tmp/ccorxZHa.o: In function `main':
figure-4.25.c:(.text+0x75): undefined reference to `minor'
figure-4.25.c:(.text+0x8b): undefined reference to `major'
figure-4.25.c:(.text+0xd4): undefined reference to `minor'
figure-4.25.c:(.text+0xea): undefined reference to `major'
collect2: ld returned 1 exit status
[fanbin@localhost apue]$ 
```
既然提示是未定义的引用，第一反应当然是没有 include 头文件，man 了下，发现包含 `<sys/types.h>` 就可以，查了一下自己的 include 确实是包含了的，至此怀疑是否本地文档过旧，查了一下在线的 [major](http://man7.org/linux/man-pages/man3/major.3.html)，发现这么一句话 `#define _BSD_SOURCE /* See feature_test_macros(7) */` ，咦？比我本地 man 多了那么个注释，好吧，我就瞅瞅这个 [feature_test_macros(7)](http://man7.org/linux/man-pages/man7/feature_test_macros.7.html)，粗暴的 `CTRL+F` 搜索了一下 `_BSD_SOURCE`，行了，人家括弧里写的明明白白：

```md
  _BSD_SOURCE (deprecated since glibc 2.20)
```
想使用我？可以啊，`glibc >= 2.20`，查我本地 glibc 版本
```ush
[fanbin@localhost apue]$ rpm -qa | grep glibc
glibc-2.12-1.166.el6_7.3.i686
glibc-devel-2.12-1.166.el6_7.3.i686
glibc-common-2.12-1.166.el6_7.3.i686
glibc-headers-2.12-1.166.el6_7.3.i686
[fanbin@localhost apue]$
```
**!!! 好吧，是我环境的锅，赶紧吓得我升级glibc，刚敲完 yum 命令，哎？!哎？!哎？!不对啊，这个玩意很重要的，不能乱搞，乱升级版本，会造成大影响的（针对业务环境），虽然我是虚拟的学习环境，但是还是抑制了冲动。**

使用更直接的方法解决问题：GOOGLE搜索框中键入 "undefined reference to `minor'"， [JasonLeaster](http://blog.csdn.net/cinmyheart/article/details/21877487) 这位大侠已经提供了解决方案，OK，轻松解决问题。

其他参考资料：
http://blog.csdn.net/cinmyheart/article/details/21877487

[BACK TO TOP](#目录)

----------------
第五章 标准I/O库
================

## 5.3 标准输入、标准输出和标准错误

`流的定向(stream's orientation)`：决定了所读、写的字符是单字节还是多字节。

对于一个进程预定义了3个流，并且这3个流可以自动地被进程使用，它们是：标准输入、标准输出和标准错误。这些流引用的文件与在 [Figure-1.4.c](#figure-14c) 及 [3.2](#xx) 节中提到的文件描述符 *STDIN_FILENO、STDOUT_FILENO和STDERR_FILENO* 所引用的相同。

**这3个标准 I/O 流通过预定义文件指针stdin、stdout和stderr加以引用。这三个文件指针定义头文件`<stdio.h>`中。**

[BACK TO TOP](#目录)

## 5.4 缓冲
标准I/O库提供缓冲的目的是尽可能减少使用 read 和 write 调用的次数（参看原书图3-6）。
标准I/O提供了一下3中类型的缓冲。

（1）**全缓冲**，在这种情况下，在填满标准I/O缓冲区后才进行实际I/O操作。对于驻留在磁盘上的文件通常是由标准I/O库实施全缓冲。

（2）**行缓冲**，在这种情况下，当在输入和输出中遇到换行符时，标准I/O库执行I/O操作。对于行缓冲有两个限制：

* 第一，因为标准I/O库用来收集每一行的缓冲区的长度是固定的，所以只要填满了缓冲区，那么及时还没有一个换行符，也进行I/O操作。
* 第二，任何时候只要通过标准I/O库要求从 (a) 或者 (b) 得到输入数据，那么就会冲洗所有行缓冲输出流。
    * (a) 一个不带缓冲的流
    * (b) 一个行缓冲的流（它从内核请求需要数据）
    
（3）**不带缓冲**，标准I/O库不对字符进行缓冲存储。

**标准错误流 [stderr](http://man7.org/linux/man-pages/man3/stderr.3.html) 通常是不带缓冲的，这就使得出错信息可以尽快的显示出来，而不管他们是否含有一个换行符。**

ISO C 要求下列缓冲特征：

* 当且仅当标准输入和标准输出并不指向交互设备时，他们才是全缓冲的。
* 标准错误绝不会是全缓冲的。

但是，这并没有具体说明什么情况下是行缓冲和不带缓冲的。**很多系统默认使用下列类型的缓冲：**

* **标准错误是不带缓冲的。**
* **若是指向终端设备的流，则是行缓冲的；否则是全缓冲的。**

使用 [setbuf](http://man7.org/linux/man-pages/man3/setbuf.3.html) 函数可以打开或关闭缓冲机制，使用 [setvbuf](http://man7.org/linux/man-pages/man3/setvbuf.3.html)，我们可以精确地说明所需的缓冲类型。

任何时候我们都可以强制冲洗一个流 [fflush](http://man7.org/linux/man-pages/man3/fflush.3.html)，作为一个特殊情况，若 `fflush` 的参数是 **NULL**，则此函数将导致所有输出流被冲洗。

### Section-5.4.c

我们编写一个简单的程序学习一下此节内容：
```c
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
```

执行结果：
```sh
[fanbin@localhost apue]$ gcc section-5.4.c
[fanbin@localhost apue]$ ./a.out 
-----------------------
waiting 2 second.
<terminal>io line buf, waiting\n... <terminal> line buf
io no buf, now!\n test
-----------------------
waiting 2 second.[fanbin@localhost apue]$ 
```

单单从执行结果其实看不出太多内容，建议实际执行一下理解。 简单描述一下执行顺序：
`#1#3=>#4=>#2#5=>#6=>#7=>#8=>#9=>#10=>#11=>#12=>#13=>#15=>#14=>#16`
|阶段|原因描述|
|:--:|:------:|
|`#1#3`|标准错误默认不带缓冲，及时输出错误信息，与前文呼应。|
|`#4`|sleep 2s|
|`#2#5`|输出流若是终端，则默认行缓冲，`#5` 输出了换行符。|
|`#6`|sleep 2s|
|`#7`|关闭标准输出的缓冲模式。|
|`#8`|非换行输出，直接输出内容，并未缓冲。|
|`#9`|sleep 2s|
|`#10`|直接输出。|
|`#11`|sleep 2s|
|`#12`|标准错误直接输出。|
|`#13`|将标准错误的非缓冲模式换成行缓冲模式。|
|`#15`|sleep 2s|
|`#14`|行缓冲，缓冲至buffer中。|
|`#16`|exit函数冲洗任何未写的数据，然后关闭所有打开的流。（[8.5节](#xx) 讨论）

[BACK TO TOP](#目录)

## 5.8 标准I/O的效率

### Figure-5.4.c
功能：使用 `getc` 和 `putc` 将标准输入复制到标准输出。
涉及头文件：[stdio.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdio.h.html)
涉及函数：[getc](http://man7.org/linux/man-pages/man3/getc.3.html)，[putc](http://man7.org/linux/man-pages/man3/putc.3.html)，[ferror](http://man7.org/linux/man-pages/man3/ferror.3.html)
沉淀内容：了解 `getc`，`putc` 由宏实现，非函数

```c
#include "apue.h"
#include <stdio.h> /* getc, putc, stdin, stdout, ferror */
#include <stdlib.h> /* exit */

/* gcc apue.h apue_err.c figure-5.4.c */
int
main(int argc, char *argv[])
{
    int c;

    /* getc, putc 被简单的实现为宏 */
    while((c = getc(stdin)) != EOF) {
        if (putc(c, stdout) == EOF) {
            err_sys("output error");
        }
    }

    if (ferror(stdin)) {
        err_sys("input error");
    }
    exit(0); /* exit函数会冲洗任何未写的数据，然后关闭所有打开的流 */
}
```

至头文件中去粗略查看一下宏定义（`/usr/include/stdio.h`）
```c
/* The C standard explicitly says this is a macro, so we always do the              
   optimization for it.  */                                                         
#define getc(_fp) _IO_getc (_fp)
...
/* The C standard explicitly says this can be a macro,                              
   so we always do the optimization for it.  */                                     
#define putc(_ch, _fp) _IO_putc (_ch, _fp) 
```

### Figure-5.5.c
功能：使用 `fgetc`，`fputc` 函数实现 [Figure-5.4.c](#figure-54c)
涉及头文件：[stdio.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdio.h.html)
涉及函数：[fgetc](http://man7.org/linux/man-pages/man3/fgetc.3.html)，[fputc](http://man7.org/linux/man-pages/man3/fputc.3.html)

```c
#include "apue.h"
#include <stdio.h> /* fgetc, fputc, stdin, stdout, ferror */
#include <stdlib.h> /* exit */

/* gcc apue.h apue_err.c figure-5.5.c */
int
main(int argc, char *argv[])
{
    char buf[MAXLINE];

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (fputs(buf, stdout) == EOF) {
            err_sys("output error");
        }
    }
    if (ferror(stdin)) {
        err_sys("input error");
    }

    exit(0); /* exit函数会冲洗任何未写的数据，然后关闭所有打开的流 */
}
```

**我们在使用标准I/O例程的一个优点是无需考虑缓冲及最佳I/O长度的选择。**
各种效率的说明参考原书 [5.8节](#xx)

[BACK TO TOP](#目录)

## 5.9 二进制I/O
```c
#incldue <stdio.h>
size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
```

常见用法：
（1）读或写一个二进制数组。
```c
float data[10];

if (fwrite(&data[2], sizeof(float), 4, fp) != 4)
    err_sys("fwrite error");
```

（2）读或写一个结构。
```c
struct {
    short   count;
    long    total;
    char    name[NAMESIZE];
} item;

if (fwrite(&ite, sizeof(item), 1, fp) != 1)
    err_sys("fwrite error")
```
两者结合起来就可读或写一个结构数组。 `fread` 和 `fwrite` 返回读或写的对象数：

* 对于读，如果出错或达到文件尾端，返回值可以少于 *nobj*。这种情况，应调用 `ferror` 或 `feof` 以判断究竟是哪一种情况。
* 对于写，如果返回值少于所要求的 *nobj*，则出错。

**使用二进制I/O的基本问题是，它只只能用于读在他同一系统上已写的数据。对于当今的很多异构系统以上两个函数不能正常工作。原因如下：**

**（1）在一个结构中，同一成员的偏移量可能随编译器程序和系统的不同而不同（由于不同的对其要求）。**

**（2）用来存储多字节整数和浮点值的二进制格式在不同的系统结构间也可能不同。**

**在不同系统之间交换二进制数据的实际解决方法是使用互认的规范格式。**（在 [第十六章](#xx) 讨论套接字时，将涉及某些相关问题）

[BACK TO TOP](#目录)

## 5.12 实现细节

每一个标准I/O流都有一个与其相关联的文件描述符，可以对一个流调用 [fileno](http://man7.org/linux/man-pages/man3/fileno.3.html) 函数以获得其描述符。

### Figure-5.11.c
功能：为3个标准流以及一个与普通文件相关联的流打印有关缓冲的状态信息。
涉及函数：[fileno](http://man7.org/linux/man-pages/man3/fileno.3.html)
沉淀内容：如何判断一个流是何种缓冲方式。

```c
#include "apue.h"
#include <stdio.h> /* FILE, EOF, getchar */
#include <stdlib.h>

/* man stdio.h 查看本机器对应的buffer类型枚举值 */

void pr_stdio(const char *, FILE *);
int  is_unbuffered(FILE *);
int  is_linebuffered(FILE *);
int  buffer_size(FILE *);

/* gcc apue.h apue_err.c figure-5.11.c */
int
main(int argc, char *argv[])
{
    FILE *fp;

    fputs("enter any character\n", stdout);
    if (getchar() == EOF) {
        err_sys("getchar error");
    }
    fputs("one line to standared error\n", stderr);

    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);

    if ((fp = fopen("/etc/passwd", "r")) == NULL) {
        err_sys("fopen error");
    }
    if (getc(fp) == EOF) {
        err_sys("getc error");
    }
    pr_stdio("/etc/passwd", fp);
    exit(0);
}

void
pr_stdio(const char *name, FILE *fp)
{
    printf("stream = %-12s, fileno = %d, ", name, fileno(fp));
    if (is_unbuffered(fp)) {
        printf("unbuffered     ");
    } else if (is_linebuffered(fp)) {
        printf("line buffered  ");
    } else { /* if neither of above */
        printf("fully buffered ");
    }
    printf(", buffer size = %d\n", buffer_size(fp));
}

/*
 * The following is nonportable.
 */

#if defined(_IO_UNBUFFERED)

int
is_unbuffered(FILE *fp)
{
    return (fp->_flags & _IO_UNBUFFERED);
}

int
is_linebuffered(FILE *fp)
{
    return (fp->_flags & _IO_LINE_BUF);
}

int
buffer_size(FILE *fp)
{
    return (fp->_IO_buf_end - fp->_IO_buf_base);
}

#elif defined(__SNBF)

int
is_unbuffered(FILE *fp)
{
    return (fp->_flags & __SNBF);
}

int
is_linebuffered(FILE *fp)
{
    return (fp->_flags & __SLBF);
}

int buffer_size(FILE *fp)
{
    return (fp->_bf._size);
}

#elif defined(_IONBF)

#ifdef _LP64
#define _flag __pad[4]
#define _ptr  __pad[1]
#define _base __pad[2]
#endif

int
is_unbuffered(FILE *fp)
{
    return (fp->_flags & _IONBF);
}

int
is_linebuffered(FILE *fp)
{
    return (fp->_flag & _IOLBF);
}

int
buffer_size(FILE *fp)
{
#ifdef _LP64
    return (fp->_base - fp->_ptr);
#else
    return (BUFSIZE); /* just a guess */
#endif
}

#else

#error unknown stdio implementataion!

#endif
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out                    # 标准输入：终端；标准输出：终端
enter any character
a
one line to standared error
stream = stdin       , fileno = 0, line buffered  , buffer size = 1024
stream = stdout      , fileno = 1, line buffered  , buffer size = 1024
stream = stderr      , fileno = 2, unbuffered     , buffer size = 1
stream = /etc/passwd , fileno = 3, fully buffered , buffer size = 4096
[fanbin@localhost apue]$ cat a.txt 
a
[fanbin@localhost apue]$ ./a.out < a.txt            # 标准输入：文件；标准输出：终端
enter any character
one line to standared error
stream = stdin       , fileno = 0, fully buffered , buffer size = 4096
stream = stdout      , fileno = 1, line buffered  , buffer size = 1024
stream = stderr      , fileno = 2, unbuffered     , buffer size = 1
stream = /etc/passwd , fileno = 3, fully buffered , buffer size = 4096
[fanbin@localhost apue]$ ./a.out > b.txt            # 标准输入：终端；标准输出：文件
a
one line to standared error
[fanbin@localhost apue]$ cat b.txt
enter any character
stream = stdin       , fileno = 0, line buffered  , buffer size = 1024
stream = stdout      , fileno = 1, fully buffered , buffer size = 4096
stream = stderr      , fileno = 2, unbuffered     , buffer size = 1
stream = /etc/passwd , fileno = 3, fully buffered , buffer size = 4096
[fanbin@localhost apue]$ ./a.out < a.txt > b.txt    # 标准输入：文件；标准输出：文件
one line to standared error
[fanbin@localhost apue]$ cat b.txt
enter any character
stream = stdin       , fileno = 0, fully buffered , buffer size = 4096
stream = stdout      , fileno = 1, fully buffered , buffer size = 4096
stream = stderr      , fileno = 2, unbuffered     , buffer size = 1
stream = /etc/passwd , fileno = 3, fully buffered , buffer size = 4096
[fanbin@localhost apue]$ 
```

通过以上执行结果，我们可了解标准输入、标准输出、标准错误默认情况下在终端，文件时的缓冲模式，及对应的buffer大小。buffer大小由系统决定，通常为磁盘块长度4096（参见 [3.9 I/O的效率](#39-io的效率)）

当标准输入、标准输出重定向到普通文件时，它们就变成是全缓冲的，其缓冲区长度是该文件系统优先选用的I/O长度（从 stat 结构中得到的 st_blksize 值，参见 [Figure-4.3.c](#figure-43c) or `man 2 stat`）。
 
## 5.13 临时文件

### Figure-5.12.c
功能：利用函数 tmpnam, temfile 创建临时文件。
涉及头文件：[stdio.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdio.h.html)
涉及函数：[tmpnam](http://man7.org/linux/man-pages/man3/tmpnam.3.html)，[tmpfile](http://man7.org/linux/man-pages/man3/tmpfile.3.html)，[rewind](http://man7.org/linux/man-pages/man3/rewind.3.html)
沉淀内容：了解标准I/O创建临时文件的方法，及函数tmpnam的缺点。

```c
#include "apue.h"
#include <stdio.h> /* L_tmpname, tmpnam, rewind, TMP_MAX */

/* gcc apue.h apue_err.c figure-5.12.c */
int
main(int argc, char *argv[])
{
    char name[L_tmpnam], line[MAXLINE];
    FILE *fp;

    printf("%s\n", tmpnam(NULL)); /* first temp name */

    /* tmpnam最多调用TMP_MAX次 */
    /* tmpnam创建文件名 */
    tmpnam(name); /* second temp name */
    printf("%s\n", name);
    printf("TMP_MAX = %d\n", TMP_MAX);

    /* tmpfile创建文件 */
    if ((fp = tmpfile()) == NULL) { /* create temp file */
        err_sys("tmpfile error");
    }
    fputs("one line of output\n", fp); /* write to temp file */
    /*
     * equivalent to : (void) fseek(stream, 0L, SEEK_SET)  
     */
    rewind(fp); /* then read it back */
    if (fgets(line, sizeof(line), fp) == NULL) {
        err_sys("fgets error");
    }
    fputs(line, stdout); /* print the line we wrote */
    
    exit(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ gcc apue.h apue_err.c figure-5.12.c
/tmp/ccj3SNsC.o: In function `main':
figure-5.12.c:(.text+0x14): warning: the use of `tmpnam' is dangerous, better use `mkstemp'
[fanbin@localhost apue]$ ./a.out 
/tmp/filedocXWG
/tmp/file1wwEcC
TMP_MAX = 238328
one line of output
[fanbin@localhost apue]$ 
```

ISO C 标准I/O提供了创建临时文件方式，**但用 `tmpnam` 和 `tempnam` 创建文件时有一个缺点：在返回唯一的路径名和用该名字创建文件之间存在一个时间窗口，在这个时间窗口中，另一进程可以用相同的名字创建文件。（非原子性操作）**

`tmpfile` 函数经常使用的标准 UNIX 技术是先调用产生一个唯一的路径名，然后，用该路径名创建一个文件，并立即 `unlink` 它（关于 `unlink` 参见 [4.14 文件系统](#414-文件系统)）。**而关闭该临时文件可以是显示的，也可以在程序终止时自动进行。**

我们在编译程序时，编译器也警告我们使用 `tmpnam` 是一个危险的操作，更合适的方式是使用 `mkstmp`，接下来我们看一个 `mkstmp` 的例子。

### Figure-5.13.c
功能：利用 mkstemp 创建临时文件。
涉及头文件：[stdlib.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdlib.h.html)
涉及函数：[mkstemp](http://man7.org/linux/man-pages/man3/mktemp.3.html)
沉淀内容：学会正确的使用 `mkstemp`

```c
#include "apue.h"
#include <stdlib.h>    /* mkstemp, */
#include <errno.h>     /* errno, ENOENT */
#include <sys/types.h> /* stat */
#include <sys/stat.h>  /* stat */
#include <unistd.h>    /* stat, unlink */

void make_temp(char *template);

/* gcc apue.h apue_err.c figure-5.12.c */
/* Single UNIX Specification为处理临时文件定义了另外两个函数：mkdtemp，mkdtemp
 * 它们是XSI的扩展部分 */
/* 原子性 */
int
main(int argc, char *argv[])
{
    /* 数据在栈上 */
    char  good_template[] = "/tmp/dirXXXXXX"; /* right way */
    /* 数据指针在栈上，数据在只读段上，无法修改 */
    char *bad_template = "/tmp/dirXXXXXX"; /* wrong way */

    printf("trying to create first temp file...\n");
    make_temp(good_template);
    printf("trying to create second temp file...\n");
    make_temp(bad_template);
    exit(0);
}

void
make_temp(char *template)
{
    int fd;
    struct stat sbuf;

    if ((fd = mkstemp(template)) < 0) {
        err_sys("Can't create temp file");
    }
    printf("temp name = %s\n", template);
    close(fd);
    if (stat(template, &sbuf) < 0) {
        if (errno == ENOENT) {
            printf("file doesn't exist\n");
        } else {
            printf("stat filed");
        }
    } else {
        printf("file exists\n");
        unlink(template); /* 不会自动删除，必须自己对它解除链接 */
    }
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out 
trying to create first temp file...
temp name = /tmp/dirCPBmhO
file exists
trying to create second temp file...
Segmentation fault
[fanbin@localhost apue]$ 
```

以上例子展示出如何利用 `mkstemp` 创建临时文件，其中有两个点需要小心：

（1）**函数 `mkstemp` 使用的模板数据需要放在栈上，而不是模板指针放在栈上。**以上例子的数据指针在栈上，但实际数据在编译后存在只读段上，函数 `mkstemp` 无法修改模板内容形成文件名称，出现段错误。

（2）**函数 `mkstemp` 创建的文件不会自动删除，我们必须使用 `unlink` 来解除对它的链接。**

[BACK TO TOP](#目录)

## 5.14 内存流

### Figure-5.15.c
功能：如何操作内存流
涉及头文件：[stdio.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdio.h.html)
涉及函数：[memset](http://man7.org/linux/man-pages/man3/memset.3.html)，[fmemopen](http://man7.org/linux/man-pages/man3/fmemopen.3.html)，[fflush](http://man7.org/linux/man-pages/man3/fflush.3.html)，[fseek](http://man7.org/linux/man-pages/man3/fseek.3.html)，[fclose](http://man7.org/linux/man-pages/man3/fclose.3.html)
沉淀内容：学会如何操作内存流数据，理解内存流。

```c
#include "apue.h"
#include <stdio.h> /* memset, fmemopen, fflush, fseek, fclose */

#define BSZ 48

/* gcc apue.h apue_err.h figure-5.15.c */
/* 内存流，更适用于字符串操作 
 * open_memstream, open_wmemstream缓冲区可增长，
 * 非常适合创建字符串 */
int
main(int argc, char *argv[])
{
    FILE *fp;
    char  buf[BSZ];

    memset(buf, 'a', BSZ-2);
    buf[BSZ-2] = '\0';
    buf[BSZ-1] = 'X';
    if ((fp = fmemopen(buf, BSZ, "w+")) == NULL) {
        err_sys("fmemopen failed");
    }
    printf("initial buffer contents: %s\n", buf);
    fprintf(fp, "hello, world");
    printf("before flush: %s\n", buf);
    fflush(fp);
    printf("after fflush: %s\n", buf);
    printf("len of string in buf = %ld\n\n", (long)strlen(buf));

    memset(buf, 'b', BSZ-2);
    buf[BSZ-2] = '\0';
    buf[BSZ-1] = 'X';
    fprintf(fp, "hello, world");
    fseek(fp, 0, SEEK_SET);
    printf("after fseek: %s\n", buf);
    printf("len of string in buf = %ld\n\n", (long)strlen(buf));

    memset(buf, 'c', BSZ-2);
    buf[BSZ-2] = '\0';
    buf[BSZ-1] = 'X';
    fprintf(fp, "hello, world");
    fclose(fp);
    printf("after fclose: %s\n", buf);
    printf("len of string in buf = %ld\n\n", (long)strlen(buf));

    return(0);
}
```

执行结果：
```sh
[fanbin@localhost apue]$ ./a.out 
initial buffer contents: 
before flush: 
after fflush: hello, world
len of string in buf = 12

after fseek: bbbbbbbbbbbbhello, world
len of string in buf = 24

after fclose: hello, worldcccccccccccccccccccccccccccccccccc
len of string in buf = 46

[fanbin@localhost apue]$
```

[BACK TO TOP](#目录)

-------------------
第十章
===================

10.15 重启动！！！，[Figure-16.19.c](#figure-1619c)

[BACK TO TOP](#目录)

-------------------
第十三章
===================

## 13.3
守护进程 => [16.5](#xx)

[BACK TO TOP](#目录)

--------------------------------------------------------------------------------
第十六章 网络IPC：套接字
================================================================================

## 16.2 套接字描述

|类型(type)|描述|
|----------|----|
|SOCK_DGRAM|**[固定长度的](http://blog.sina.com.cn/s/blog_46d93f190100ojap.html)**、无连接的、不可靠的报文传递|
|SOCK_RAW|IP协议的数据报接口（在 POSIX.1 中为可选）|
|SOCK_SEQPACKET|固定长度的、有序的、可靠的、面向连接的报文传递|
|SOCK_STREAM|有序的、可靠的、双向的、面向连接的字节流|

***SOCK_STREAM*** 套接字提供字节流服务，所以应用程序 **分辨不出报文的界限** 。
这意味着从 *SOCK_STREAM* 套接字读数据时，它也许不会返回所有由发送进程所写的字节
数。最终可以获得发送过来的所有数据，但也许要通过 **若干次函数调用** 才能得到。

[MTU](https://en.wikipedia.org/wiki/Maximum_transmission_unit 'maximum transmission unit - wikipedia')：1500， 576

套接字通信是双向的。可以采用 [shutdown(2)](http://man7.org/linux/man-pages/man2/shutdown.2.html) 
函数来禁止一个套接字的I/O。

```c

 #include <sys/socket.h>
 int shutdown(int sockfd, int how);
```

| *how* |作用|
|:-----:|----|
|SHUT_RD|关闭读端，无法从套接字读取数据|
|SHUT_WR|关闭写端，无法使用套接字发送数据|
|SHUT_RDWR|关闭读写，既无法读取数据，也无法发送数据|

能够关闭（`close`）一个套接字，为何还使用 `shutdown` 呢？这里有若干理由。 首先，
只有最后一个活动引用关闭时，`close` 才释放网络端点。这意味着如果复制一个 套接字
（如采用 `dup`），要直到关闭了最后一个引用它的文件描述符才会释放这个套接 字。**
而 `shutdown` 允许使一个套接字处于不活动状态，和引用它的文件描述符数目无 关。**
其次，有时可以很方便地关闭套接字双向传输中的一个方向。例如，如果想让所通信的进程
能够确定数据传输何时结束，可以关闭该套接字的写端，然而通过该套接字读端仍可以继续
接收数据。

其他参考资料：

http://blog.sina.com.cn/s/blog_46d93f190100ojap.html

[BACK TO TOP](#目录)

## 16.3 寻址

### 16.3.3 地址查询

#### Figure-16.9.c

功能：`getaddrinfo` 的用法介绍

涉及头文件：
[netinet/in.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netinet_in.h.html)，
[netdb.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netdb.h.html)，
[arpa/inet.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/arpa_inet.h.html)

涉及函数：
[getaddrinfo(3)](http://man7.org/linux/man-pages/man3/getaddrinfo.3.html)

沉淀内容：`getaddrinfo` 用法，`/etc/hosts`，`/etc/services`

```c

 #include "apue.h"
 #if defined(SOLARIS)
 #include <netinet/in.h>
 #endif
 #include <netdb.h>
 #include <arpa/inet.h>
 #if defined(BSD)
 #include <sys/socket.h>
 #include <netinet/in.h>
 #endif
 
 void
 print_family(struct addrinfo *aip)
 {
     printf(" family ");
     switch (aip->ai_family) {
     case AF_INET:
         printf("inet");
         break;
     case AF_INET6:
         printf("inet6");
         break;
     case AF_UNIX:
         printf("unix");
         break;
     case AF_UNSPEC:
         printf("unspecified");
         break;
     default:
         printf("unknown");
     }
 }
 
 void
 print_type(struct addrinfo *aip)
 {
     printf(" type ");
     switch (aip->ai_socktype) {
     case SOCK_STREAM:
         printf("stream");
         break;
     case SOCK_DGRAM:
         printf("datagram");
         break;
     case SOCK_SEQPACKET:
         printf("seqpacket");
         break;
     case SOCK_RAW:
         printf("raw");
         break;
     default:
         printf("unknown (%d)", aip->ai_socktype);
     }
 }
 
 void
 print_protocol(struct addrinfo *aip)
 {
     printf(" protocol ");
     switch (aip->ai_protocol) {
     case 0:
         printf("default");
         break;
     case IPPROTO_TCP:
         printf("TCP");
         break;
     case IPPROTO_UDP:
         printf("UDP");
         break;
     case IPPROTO_RAW:
         printf("raw");
         break;
     default:
         printf("unknown (%d)", aip->ai_protocol);
     }
 }
 
 void
 print_flags(struct addrinfo *aip)
 {
     printf("flags");
     if (aip->ai_flags == 0) {
         printf(" 0");
     } else {
         if (aip->ai_flags & AI_PASSIVE)
             printf(" passive");
         if (aip->ai_flags & AI_CANONNAME)
             printf(" canon");
         if (aip->ai_flags & AI_NUMERICHOST)
             printf(" numhost");
         if (aip->ai_flags & AI_NUMERICSERV)
             printf(" numserv");
         if (aip->ai_flags & AI_V4MAPPED)
             printf(" v4mapped");
         if (aip->ai_flags & AI_ALL)
             printf(" all");
     }
 }
 
 /* gcc apue.h apue_err.c figure-16.9.c */
 int
 main(int argc, char *argv[])
 {
     struct addrinfo     *ailist, *aip;
     struct addrinfo      hint;
     struct sockaddr_in  *sinp;
     const char          *addr;
     int                  err;
     char                 abuf[INET_ADDRSTRLEN];
 
     if (argc != 3)
         err_quit("usage: %s nodename service", argv[0]);
     hint.ai_flags = AI_CANONNAME;
     hint.ai_family = 0;
     hint.ai_socktype = 0;
     hint.ai_protocol = 0;
     hint.ai_addrlen = 0;
     hint.ai_canonname = NULL;
     hint.ai_addr = NULL;
     hint.ai_next = NULL;
     if ((err = getaddrinfo(argv[1], argv[2], &hint, &ailist)) != 0)
         err_quit("getaddrinfo error: %s", gai_strerror(err));
     for (aip = ailist; aip != NULL; aip = aip->ai_next) {
         print_flags(aip);
         print_family(aip);
         print_type(aip);
         print_protocol(aip);
         printf("\n\thost %s", aip->ai_canonname ? aip->ai_canonname:"-");
         if (aip->ai_family == AF_INET) {
             sinp = (struct sockaddr_in *)aip->ai_addr;
             addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
             printf(" address %s", addr ? addr : "unknown");
             printf(" port %d", ntohs(sinp->sin_port));
         }
         printf("\n");
     }
     exit(0);
 }
```

执行结果：

```sh

 [fanbin@localhost apue]$ ./a.out localhost nfs          # 可在 /etc/services 查询
 flags canon family inet type stream protocol TCP
         host localhost.localdomain address 127.0.0.1 port 2049
 flags canon family inet type datagram protocol UDP
         host - address 127.0.0.1 port 2049
 flags canon family inet type stream protocol unknown (132)
         host - address 127.0.0.1 port 2049
 flags canon family inet type seqpacket protocol unknown (132)
         host - address 127.0.0.1 port 2049
 [fanbin@localhost apue]$ 
```

在 `getaddrinfo` 的定义中，*node* 节点涉及 `/etc/hosts` 文件， *service* 涉及 
`/etc/services`，若使用此函数，那么对应的服务需要添加到 `/etc/services` 中以便
使用。

|文件|作用|
|:--:|----|
|`/etc/services`|存储服务与端口号的对应关系|
|`/etc/hosts`|存储地址与主机名的对应关系|

```sh

 [fanbin@localhost apue]$ cat /etc/hosts
 127.0.0.1               localhost.localdomain localhost
 ::1             localhost6.localdomain6 localhost6
 [fanbin@localhost apue]$ cat /etc/services
 ...
 ftp             21/tcp
 ftp             21/udp          fsp fspd
 ssh             22/tcp                          # The Secure Shell (SSH) Protocol
 ssh             22/udp                          # The Secure Shell (SSH) Protocol
 telnet          23/tcp
 telnet          23/udp
 ...
 [fanbin@localhost apue]$
```

端口号的使用（摘录至 [百度百科](http://baike.baidu.com/link?url=dnX_XbfcfkpkhEAakBiqwWZVmmwd5cATlXY63x65uLzxObsVHsSpUJX4JCKR6G3o)），还可参考 [Wikipedia](https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers)

> TCP与UDP段结构中端口地址都是16比特，可以有在0~65535范围内的端口号。对于这65536个端口号有以下的使用规定：
（1）端口号小于256的定义为常用端口，服务器一般都是通过常用端口号来识别的。任何TCP/IP实现所提供的服务都用1~1023之间的端口号，是由ICANN来管理的；
（2）客户端只需保证该端口号在本机上是惟一的就可以了。客户端口号因存在时间很短暂又称临时端口号；
（3）大多数TCP/IP实现给临时端口号分配1024~5000之间的端口号。大于5000的端口号是为其他服务器预留的。

其他参考资料：

http://blog.chinaunix.net/uid-20196318-id-172427.html

http://baike.baidu.com/link?url=dnX_XbfcfkpkhEAakBiqwWZVmmwd5cATlXY63x65uLzxObsVHsSpUJX4JCKR6G3o

[BACK TO TOP](#目录)

## 16.4 建立连接

将一个客户端的套接字关联上一个地址没有多少新意，可以让系统选一个默认的地址。
然而，对于服务器，需要给一个接受客户端请求的服务器套接字关联上一个 **众所周知** 
的地址。客户端有一种方法来发现连接服务器所需要的地址，最简单的方法就是服务器保留
一个地址并且注册在 `/etc/services` 或者某个名字服务中

在 `connect` 中指定的地址是我们想与之通信的服务器地址。如果 sockfd 没有绑定到一
个地址，`connect` 会给调用者绑定一个默认地址。

**当尝试连接服务器时，出于一些原因，连接可能会失败。**要想一个连接请求成功，要连
接的计算机必须是 **开启的**，并且 **正在运行**，服务器并需 **绑定** 到一个想与之
连接的地址上，并且服务器的 **等待连接队列要有足够的空间**。因此，应用程序必须能
够处理 `connect` 返回的错误，这些错误可能是由一些瞬时条件引起的。

`connect` 函数还可以用于无连接的网络服务（SOCK_DGRAM）。这点看起来有点矛盾，实际
上却是一个不错的选择。如果用 SOCK_DGRAM 套接字调用 `connect`，传送的报文的目标地
址会设置成 `connect` 调用中所指定的地址，这样每次传送报文时就不需要再提供地址。
另外，仅能接受来自指定地址的报文。 

### Figure-16.10.c

功能：处理瞬时 `connect` 错误的方法。如果一个服务器运行在负载很重的系统上，可能会发生这些错误。

涉及头文件：
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[connect(2)](http://man7.org/linux/man-pages/man2/connect.2.html)

沉淀内容：了解建立连接时的错误处理方式和 [指数补偿算法](https://en.wikipedia.org/wiki/Exponential_backoff, 'Exponential backoff - wikipedia')（参见 [1.7 出错处理](#17-出错处理)）

```c

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
```

以上代码虽然可用，但仍存在问题：代码是不可移植的。它在 Linux 和 Solaris 上可以工
作，但是在 FreeBSD 和 Mac OS X 上却不能按预期工作。在基于 BSD 的套接字实现中，如
果第一次连接尝试失败，那么在 TCP 中继续使用同一个套接字描述符，接下来仍旧会失败。
**这就是一个协议相关的行为从（协议无关的）套接字接口中显露出来变得应用程序可见的
例子。**

因此，如果 `connect` 失败，可迁移的应用程序需要关闭套接字。如果想重试，必须打开
一个新的套接字。例：[Figure-16.11.c](#figure-1611c)

Note：关于 Linux, Solaris, FresBSD, Mac OS X 的关系，参见 
[Wikipedia - Linux](https://en.wikipedia.org/wiki/Linux#Development 'linux - wikipedia')

### Figure-16.11.c

沉淀内容：可移植的 `connect_retry`。

```c

 #include "apue.h"
 #include <sys/socket.h>
 
 #define MAXSLEEP 128
 
 int
 connect_retry(int domain, int type, int protocol, const struct sockaddr *addr,
               socklen_t alen)
 {
     int     numsec, fd;
 
     /*
      * Try to connect with exponential backoff.
      */
     for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
         if ((fd = socket(domain, type, protocol)) < 0)
             return(-1);
         if (connect(fd, addr, alen) == 0) {
             /*
              * connection accepted.
              */
             return(fd);
         }
         close(fd);
 
         /*
          * Delay before trying again.
          */
         if (numsec <= MAXSLEEP / 2)
             sleep(numsec);
     }
     return(-1);
 }
```

客户端发出 `connect` 连接请求后，服务器可调用 `listen` 函数来宣告它愿意接受连接
请求。

```c

 #include <sys/socket.h>
 int listen(int sockfd, int backlog);
```

**参数 *backlog* 提供了一个提示，提示系统该进程所要入队的未完成连接请求数量。**
其实际值由系统决定，但上限由 `<sys/socket.h>` 中的 SOMAXCONN 指定。

**一旦队列满，系统就会拒绝多余的连接请求，所以 *backlog* 的值应该基于服务器期望
负载和处理量来选择，其中处理量是指接受连接请求与启动服务的数量。**

TCP 的连接过程流程参见 [wikipedia - Berkeley sockets](https://en.wikipedia.org/wiki/Berkeley_sockets)

### Figure-16.12.c

功能：分配和初始化套接字供服务器进程使用。

涉及头文件：
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[socket(2)](http://man7.org/linux/man-pages/man2/socket.2.html)，
[bind(2)](http://man7.org/linux/man-pages/man2/bind.2.html)，
[listen(2)](http://man7.org/linux/man-pages/man2/listen.2.html)

沉淀内容：学会 socket 编程的基本步骤

```c

 #include "apue.h"
 #include <errno.h>
 #include <sys/socket.h>
 
 int
 initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
 {
     int     fd;
     int     err = 0;
 
     if ((fd = socket(addr->sa_family, type, 0)) < 0)
         return(-1);
     if (bind(fd, addr, alen) < 0)
         goto errout;
     if (type == SOCK_STREAM || type == SOCK_SETPACKET) {
         if (listen(fd, qlen) < 0)
             goto errout;
     }
     return(fd);
 
 errout:
     err = errno;
     close(fd);
     errno = err;
     return(-1);
 }
```

TCP 有一些奇怪的地址复用规则，这使得 **这个例子并不完备。** [Figure-16.22.c](#figure-1622c) 显示了有关这个函数的另一个版本，可以绕过这些规则，解决此版本的主要缺陷。

[BACK TO TOP](#目录)

## 16.5 数据传输

既然一个套接字端点表示为一个文件描述符，那么只要建立连接，就可以使用 `read` 和 
`write`来通过套接字通信。**在套接字描述符上使用 `read` 和 `write` 是非常有意义
的，因为这意味着可以将套接字描述符传递给那些原先为处理本地文件而设计的函数。**而
且还可以安排将套接字描述符传递给子进程，而该子进程执行的程序并不了解套接字。

对于支持报文边界的协议（如：UDP），如果尝试发送的单个报文的长度超过协议所支持的
最大长度，那么 `send` 会失败，并将 `errno` 设置为 *EMSGSIZE*。对于字节流协议（如
：TCP），`send` 会阻塞直到整个数据传输完成。

### Figure-16.16.c（面向连接的客户端）

功能：与服务器通信的客户端从服务器系统的 `uptime` 命令获得输出。- "远程正常运行时间"

涉及头文件：
[netdb.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netdb.h.html)，
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[recv(2)](http://man7.org/linux/man-pages/man2/recv.2.html)，
[getaddrinfo(3)](http://man7.org/linux/man-pages/man3/getaddrinfo.3.html)

沉淀内容：加深对 `getaddrinfo` 的理解

```c

 #include "apue.h"
 #include <netdb.h>
 #include <errno.h>
 #include <sys/socket.h>
 
 #define BUFLEN  128
 
 extern int connect_retry(int, int, int, const struct sockaddr *, socklen_t);
 
 void
 print_uptime(int sockfd)
 {
     int     n;
     char    buf[BUFLEN];
 
     while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
         write(STDOUT_FILENO, buf, n);
     if (n < 0)
         err_sys("recv error");
 }
 
 /* gcc apue.h apue_err.c figure-16.11.c figure-16.16.c -o ruptime */
 int
 main(int argc, char *argv[])
 {
     struct addrinfo     *ailist, *aip;
     struct addrinfo      hint;
     int                  sockfd, err;
 
     if (argc != 2)
         err_quit("usage: ruptime hostname");
     memset(&hint, 0, sizeof(hint));
     hint.ai_socktype = SOCK_STREAM;
     hint.ai_canonname = NULL;
     hint.ai_addr = NULL;
     hint.ai_next = NULL;
     if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
         err_quit("getaddrinfo error: %s", gai_strerror(err));
     for (aip = ailist; aip != NULL; aip = aip->ai_next) {
         if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0,
                         aip->ai_addr, aip->ai_addrlen)) < 0) {
             err = errno;
         } else {
             print_uptime(sockfd);
             exit(0);
         }
     }
     err_exit(err, "can't connect to %s", argv[1]);
 }
```

### Figure-16.17.c（面向连接的服务器）

功能：向连接的客户端发送 `uptime` 命令的输出。

涉及头文件：
[netdb.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netdb.h.html)，
[syslog.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/syslog.h.html)，
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[accept(2)](http://man7.org/linux/man-pages/man2/accept.2.html)，
[popen(3)](http://man7.org/linux/man-pages/man3/popen.3.html)，
[send(2)](http://man7.org/linux/man-pages/man2/send.2.html)，
[pclose(3)](http://man7.org/linux/man-pages/man3/pclose.3.html), 
[gethostname(2)](http://man7.org/linux/man-pages/man2/gethostname.2.html)，
[getaddrinfo(3)](http://man7.org/linux/man-pages/man3/getaddrinfo.3.html)，
[syslog(2)](http://man7.org/linux/man-pages/man2/syslog.2.html)

沉淀内容：温习管道内容，系统日志内容；学会使用 `getaddrinfo` 及其服务配置。

```c

 #include "apue.h"
 #include <netdb.h>
 #include <errno.h>
 #include <syslog.h>
 #include <sys/socket.h>
 
 #define BUFLEN  128
 #define QLEN    10
 
 #ifndef HOST_NAME_MAX
 #define HOST_NAME_MAX 256
 #endif
 
 /* figure-16.12.c => figure-16.22.c */
 extern int initserver(int, const struct sockaddr *, socklen_t, int);
 
 void
 serve(int sockfd)
 {
     int     clfd;
     FILE   *fp;
     char    buf[BUFLEN];
 
     set_cloexec(sockfd);    /* figure-13.9.c */
     for (;;) {
         if ((clfd = accept(sockfd, NULL, NULL)) < 0) {
             syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
             exit(1);
         }
         set_cloexec(clfd);
         if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
             sprintf(buf, "error: %s\n", strerror(errno));
             send(clfd, buf, strlen(buf), 0);
         } else {
             while (fgets(buf, BUFLEN, fp) != NULL)
                 send(clfd, buf, strlen(buf), 0);
             pclose(fp);
         }
         close(clfd);
     }
 }
 
 /* gcc apue.h apue_err.c figure-13.1.c figure-13.9.c figure-16.22.c figure-16.17.c -o server */
 int
 main(int argc, char *argv[])
 {
     struct addrinfo     *ailist, *aip;
     struct addrinfo      hint;
     int                  sockfd, err, n;
     char                *host;
 
     if (argc != 1)
         err_quit("usage: ruptime");
     if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
         n = HOST_NAME_MAX;  /* best guess */
     if ((host = malloc(n)) == NULL)
         err_sys("malloc error");
     if (gethostname(host, n) < 0)
         err_sys("gethostname error");
     daemonize("ruptimed");
     memset(&hint, 0, sizeof(hint));
     hint.ai_flags = AI_CANONNAME;
     hint.ai_socktype = SOCK_STREAM;
     hint.ai_canonname = NULL;
     hint.ai_addr = NULL;
     hint.ai_next = NULL;
     if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0) {
         syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));
         exit(1);
     }
     for (aip = ailist; aip != NULL; aip = aip->ai_next) {
         if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr,
                         aip->ai_addrlen, QLEN)) >= 0) {
             serve(sockfd);
             exit(0);
         }
     }
     exit(1);
 }
```

执行结果：

```sh

 [fanbin@localhost apue]$ ./server
 [fanbin@localhost apue]$ ./ruptime localhost
  16:32:04 up 47 days,  2:48,  3 users,  load average: 0.01, 0.01, 0.00
 [fanbin@localhost apue]$ 
```

若在执行时遇到如下错误：

```sh

 Mar 14 16:19:01 localhost ruptimed: ruptimed: getaddrinfo error: Servname not supported for ai_socktype
```

请参考 [yjie_life](http://blog.163.com/yjie_life/blog/static/16319833720110311528528/) 
博客所写的解决方案及 [andyxie407](http://blog.csdn.net/andyxie407/article/details/1672325) 
对 `getaddrinfo` 的理解。

### Figure-16.18.c

功能：向连接的客户端发送 `uptime -V` 命令的输出。

涉及头文件：
[netdb.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netdb.h.html)，
[syslog.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/syslog.h.html)，
[fcntl.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/fcntl.h.html)，
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)，
[sys/wait.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_wait.h.html)

涉及函数：
[accept(2)](http://man7.org/linux/man-pages/man2/accept.2.html)，
[fork(2)](http://man7.org/linux/man-pages/man2/fork.2.html)，
[dup2(2)](http://man7.org/linux/man-pages/man2/dup2.2.html)，
[sysconf(3)](http://man7.org/linux/man-pages/man3/sysconf.3.html)，
[getaddrinfo(3)](http://man7.org/linux/man-pages/man3/getaddrinfo.3.html)，
[gethostname(2)](http://man7.org/linux/man-pages/man2/gethostname.2.html)

沉淀内容：**学会将标准输入，标注输出，标准错误绑定到 socket 套接字，理解用文件描
述符来访问套接字**

```c

 #include "apue.h"
 #include <netdb.h>
 #include <errno.h>
 #include <syslog.h>
 #include <fcntl.h>
 #include <sys/socket.h>
 #include <sys/wait.h>
 
 #define QLEN    10
 
 #ifndef HOST_NAME_MAX
 #define HOST_NAME_MAX   256
 #endif
 
 /* figure-16.12.c => figure-16.22.c */
 extern int initserver(int, const struct sockaddr *, socklen_t, int);
 
 void
 serve(int sockfd)
 {
     int     clfd, status;
     pid_t   pid;
 
     set_cloexec(sockfd);    /* figure-13.9.c */
     for (;;) {
         if ((clfd = accept(sockfd, NULL, NULL)) < 0) {
             syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
             exit(1);
         }
         if ((pid = fork()) < 0) {
             syslog(LOG_ERR, "ruptimed: fork error: %s", strerror(errno));
             exit(1);
         } else if (pid == 0) {  /* child */
             /*
              * The parent called daemonize (Figure 13.1), so
              * STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO
              * are already open to /dev/null. Thus, the call to
              * close doesn't need to be protected by checks that
              * clfd isn't already equal to one of these values.
              */
             if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO ||
                     dup2(clfd, STDERR_FILENO) != STDERR_FILENO) {
                 syslog(LOG_ERR, "ruptimed: unexpected error");
                 exit(1);
             }
             close(clfd);
             execl("/usr/bin/uptime", "uptime", "-V", (char *)0);
             syslog(LOG_ERR, "ruptimed: unexpected return from exec: %s",
                     strerror(errno));
         } else {        /* parent */
             close(clfd);
             waitpid(pid, &status, 0);
         }
     }
 }
 
 /* gcc apue.h apue_err.c figure-13.1.c figure-13.9.c figure-16.22.c figure-16.18.c -o server */
 int
 main(int argc, char *argv[])
 {
     struct addrinfo     *ailist, *aip;
     struct addrinfo      hint;
     int                  sockfd, err, n;
     char                *host;
 
     if (argc != 1)
         err_quit("usage: ruptimed");
     if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
         n = HOST_NAME_MAX;  /* best guess */
     if ((host = malloc(n)) == NULL)
         err_sys("malloc error");
     if (gethostname(host, n) < 0)
         err_sys("gethostname error");
     daemonize("ruptimed");                      /* figure-13.1.c */
     memset(&hint, 0, sizeof(hint));
     hint.ai_flags = AI_CANONNAME;
     hint.ai_socktype = SOCK_STREAM;
     hint.ai_canonname = NULL;
     hint.ai_addr = NULL;
     hint.ai_next = NULL;
     if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0) {
         syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));
         exit(1);
     }
     for (aip = ailist; aip != NULL; aip = aip->ai_next) {
         if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr,
                         aip->ai_addrlen, QLEN)) >= 0) {
             serve(sockfd);
             exit(0);
         }
     }
     exit(1);
 }
```

执行结果：

```sh

 [fanbin@localhost apue]$ ./server               # 刻意更改命令效果，以区分上一个server
 [fanbin@localhost apue]$ ./ruptime localhost
 procps version 3.2.8
 [fanbin@localhost apue]$
```

当忘记上一个创建的守护进程时，通过查看端口号的占用情况，查出对应的 `pid`

```sh

 [fanbin@localhost apue]$ netstat -tulp
 (Not all processes could be identified, non-owned process info
  will not be shown, you would have to be root to see it all.)
 Active Internet connections (only servers)
 Proto Recv-Q Send-Q Local Address               Foreign Address             State       PID/Program name   
 tcp        0      0 *:mysql                     *:*                         LISTEN      -
 tcp        0      0 *:6379                      *:*                         LISTEN      17279/./src/redis-s 
 tcp        0      0 localhost.localdoma:ruptime *:*                         LISTEN      7945/./server       
 tcp        0      0 *:6380                      *:*                         LISTEN      17282/./src/redis-s 
 tcp        0      0 *:http                      *:*                         LISTEN      -
 tcp        0      0 *:ssh                       *:*                         LISTEN      -
 tcp        0      0 *:6379                      *:*                         LISTEN      17279/./src/redis-s 
 tcp        0      0 *:6380                      *:*                         LISTEN      17282/./src/redis-s 
 tcp        0      0 *:ssh                       *:*                         LISTEN      -
 udp        0      0 *:bootpc                    *:*                                     -
 [fanbin@localhost apue]$ 
```

**前面说过，采用文件描述符来访问套接字是非常有意义的，因为它允许程序对联网环境的
网络访问一无所知。**

前面的实例采用的都是面向连接的套接字。如何选择合适的套接字类型？何时采用面向连接
的套接字？何时采用无连接的套接字？取决于我们要做的工作量和能够容忍的出错程度。

**面向连接的套接字优点不用多说，但其缺陷在于需要更多的时间和工作来建立一个连接，
并且每个连接都需要较多的操作系统资源。**因此若能提供一个**连接池**，那么能极大的
提升系统的性能，关于连接池概念及其基本实现以后的学习再记录。

其他参考资料：

http://blog.163.com/yjie_life/blog/static/16319833720110311528528/

http://blog.csdn.net/andyxie407/article/details/1672325

### Figure-16.19.c（无连接的客户端）

功能：采用数据报套接字接口的 uptime 客户端

涉及头文件：
[netdb.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netdb.h.html)，
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[sendto(2)](http://man7.org/linux/man-pages/man2/sendto.2.html)，
[recvfrom(2)](http://man7.org/linux/man-pages/man2/recvfrom.2.html)，
[alarm(2)](http://man7.org/linux/man-pages/man2/alarm.2.html)，
[sigemptyset(3)](http://man7.org/linux/man-pages/man3/sigemptyset.3.html)，
[sigaction(2)](http://man7.org/linux/man-pages/man2/sigaction.2.html)，
[getaddrinfo(3)](http://man7.org/linux/man-pages/man3/getaddrinfo.3.html)，
[socket(2)](http://man7.org/linux/man-pages/man2/socket.2.html)

沉淀内容：学会基本的无连接客户端，温习信号机制，`alarm` 的用法及 [重启动](#10.5) 概念

```c

 #include "apue.h"
 #include <netdb.h>
 #include <errno.h>
 #include <sys/socket.h>
 
 #define BUFLEN  128
 #define TIMEOUT 20
 
 void
 sigalrm(int signo)
 {
 }
 
 void
 print_uptime(int sockfd, struct addrinfo *aip)
 {
     int     n;
     char    buf[BUFLEN];
 
     buf[0] = 0;
     if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0)
         err_sys("sendto error");
     alarm(TIMEOUT); /* 用于产生 SIGALRM 中断 */
     if ((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) < 0) {
         if (errno != EINTR)
             alarm(0);           /* 取消 SIGALRM 中断 */
         err_sys("recv error");
     }
     alarm(0);   /* 取消 SIGALRM 中断 */
     write(STDOUT_FILENO, buf, n);
 }
 
 /* gcc apue.h apue_err.c figure-16.19.c -o ruptime */
 int
 main(int argc, char *argv[])
 {
     struct addrinfo     *ailist, *aip;
     struct addrinfo      hint;
     int                  sockfd, err;
     struct sigaction     sa;
 
     if (argc != 2)
         err_quit("usage: ruptime hostname");
     sa.sa_handler = sigalrm;
     sa.sa_flags = 0;
     sigemptyset(&sa.sa_mask);
     if (sigaction(SIGALRM, &sa, NULL) < 0)
         err_sys("sigaction error");
     memset(&hint, 0, sizeof(hint));
     hint.ai_socktype = SOCK_DGRAM;
     hint.ai_canonname = NULL;
     hint.ai_addr = NULL;
     hint.ai_next = NULL;
     if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
         err_quit("getaddrinfo error: %s", gai_strerror(err));
 
     for (aip = ailist; aip != NULL; aip = aip->ai_next) {
         if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0) {
             err = errno;
         } else {
             print_uptime(sockfd, aip);
             exit(0);
         }
     }
 
     fprintf(stderr, "can't contact %s: %s\n", argv[1], strerror(err));
     exit(1);
 }
```

以上例子除了增加安装一个 *SIGALRM* 的信号处理程序以外，基于数据报的客户端中的 
main 函数和面向连接的客户端中的类似。

如果服务器不在运行状态，客户端调用 `recvfrom` 便会无限期阻塞。对于这个面向连接的
实例，如果服务器不运行，connect调用会失败。为了避免无限期阻塞，可以在调用 
`recvfrom` 之前设置警告时钟。关于 `alarm` 的使用可参考 [这个](http://liuzhigong.blog.163.com/blog/static/178272375201172021328123/) 
博客的介绍，或者至 [10.5](#xx)、[10.6](#xx) 了解 **重启动** 概念。

执行结果：

```sh

 [fanbin@localhost apue]$ ./ruptime localhost
 recv error: Interrupted system call
 [fanbin@localhost apue]$ 
```

### Figure-16.20.c（无连接的服务器）

功能：uptime 的无连接的服务器

涉及头文件：
[netdb.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netdb.h.html)，
[syslog.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/syslog.h.html)，
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[recvfrom(2)](http://man7.org/linux/man-pages/man2/recvfrom.2.html)，
[popen(3)](http://man7.org/linux/man-pages/man3/popen.3.html)，
[sendto(2)](http://man7.org/linux/man-pages/man2/sendto.2.html)，
[sysconf(3)](http://man7.org/linux/man-pages/man3/sysconf.3.html)，
[getaddrinfo(3)](http://man7.org/linux/man-pages/man3/getaddrinfo.3.html)

沉淀内容：学会写无连接的服务器

```c

 #include "apue.h"
 #include <netdb.h>
 #include <errno.h>
 #include <syslog.h>
 #include <sys/socket.h>
 
 #define BUFLEN      128
 #define MAXADDRLEN  256
 
 #ifndef HOST_NAME_MAX
 #define HOST_NAME_MAX   256
 #endif
 
 /* figure-16.12.c => figure-16.22.c */
 extern int initserver(int, const struct sockaddr *, socklen_t, int);
 
 void
 serve(int sockfd)
 {
     int              n;
     socklen_t        alen;
     FILE            *fp;
     char             buf[BUFLEN];
     char             abuf[MAXADDRLEN];
     struct sockaddr *addr = (struct sockaddr *)abuf;
 
     set_cloexec(sockfd);    /* figure-13.9.c */
     for (;;) {
         alen = MAXADDRLEN;
         if ((n = recvfrom(sockfd, buf, BUFLEN, 0, addr, &alen)) < 0) {
             syslog(LOG_ERR, "ruptimed: recvfrom error: %s", strerror(errno));
             exit(1);
         }
         if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
             sprintf(buf, "error: %s\n", strerror(errno));
             sendto(sockfd, buf, strlen(buf), 0, addr, alen);
         } else {
             if (fgets(buf, BUFLEN, fp) != NULL)
                 sendto(sockfd, buf, strlen(buf), 0, addr, alen);
             pclose(fp);
         }
     }
 }
 
 /* apue.h apue_err.c figure-13.1.c figure-13.9.c figure-16.22.c figure-16.20.c -o ruptimed */
 int
 main(int argc, char *argv[])
 {
     struct addrinfo     *ailist, *aip;
     struct addrinfo      hint;
     int                  sockfd, err, n;
     char                *host;
 
     if (argc != 1)
         err_quit("usage: ruptimed");
     if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
         n = HOST_NAME_MAX;
     if ((host = malloc(n)) == NULL)
         err_sys("malloc error");
     if (gethostname(host, n) < 0)
         err_sys("gethostname error");
     daemonize("ruptimed");                  /* figure-13.1.c */
     memset(&hint, 0, sizeof(hint));
     hint.ai_flags = AI_CANONNAME;
     hint.ai_socktype = SOCK_DGRAM;
     hint.ai_canonname = NULL;
     hint.ai_addr = NULL;
     hint.ai_next = NULL;
     if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0) {
         syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));
         exit(1);
     }
     for (aip = ailist; aip != NULL; aip = aip->ai_next) {
         if ((sockfd = initserver(SOCK_DGRAM, aip->ai_addr,
                         aip->ai_addrlen, 0)) >= 0) {
             serve(sockfd);
             exit(0);
         }
     }
     exit(1);
 }
```

其他参考资料：

http://blog.163.com/yjie_life/blog/static/16319833720110311528528/

http://wbwk2005.blog.51cto.com/2215231/400260

http://liuzhigong.blog.163.com/blog/static/178272375201172021328123/

[BACK TO TOP](#目录)

## 16.6 套接字选项

### Figure-16.22.c

功能：分配和初始化套接字供服务器进程使用，解决 [Figure-16.12.c](#figure-1612c)
 中的问题

涉及头文件：
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[socket(2)](http://man7.org/linux/man-pages/man2/socket.2.html)，
[setsockopt(2)](http://man7.org/linux/man-pages/man2/setsockopt.2.html)，
[bind(2)](http://man7.org/linux/man-pages/man2/bind.2.html)

沉淀内容：理解如何使用 `setsockopt` 及此函数能够解决什么问题。

```c

 #include "apue.h"
 #include <errno.h>
 #include <sys/socket.h>
 
 int
 initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
 {
     int     fd, err;
     int     reuse = 1;
 
     if ((fd = socket(addr->sa_family, type, 0)) < 0)
         return(-1);
     if (setsockopt(fd, SOL_SOCKET, SO_REUSERADDR, &reuse, sizeof(int)) < 0)
         goto errout;
     if (bind(fd, addr, alen) < 0)
         goto errout;
     if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
         if (listen(fd, qlen) < 0)
             goto errout;
     return(fd);
 
 errout:
     err = errno;
     close(fd);
     errno = err;
     return(-1);
 }
```

**当服务器终止并尝试立即重启时，[Figure-16.12.c](#figure-1612c) 中的函数无法正常
工作。**通常情况下，除非超时（超时时间一般是几分钟），否则 TCP 的实现不允许绑定
同一个地址。幸运的是，套接字选项 *SO_REUSEADDR* 可以绕过这个限制。

关于 `setsockopt` 能解决的问题及各个参数的含义可参考 
[未语愁眸](http://blog.csdn.net/chary8088/article/details/2486377) 的博客，写的
非常详细。

其他参考资料：

http://blog.csdn.net/chary8088/article/details/2486377

[BACK TO TOP](#目录)

## 习题

### Figure-C.22.c

功能：判断机器的字节序

```c

 #include <stdio.h>
 #include <stdlib.h>
 #include <inttypes.h>
 
 int
 main(int argc, char *argv[])
 {
     uint32_t        i = 0x04030201;
     unsigned char  *cp = (unsigned char *)&i;
 
     if (*cp == 1) {
         printf("litte-endian\n");
     } else if (*cp == 4) {
         printf("bid-endian\n");
     } else {
         printf("who knows?\n");
     }
     exit(0);
 }
```

执行结果：

```sh

 [fanbin@localhost apue]$ ./a.out 
 litte-endian
 [fanbin@localhost apue]$ 
```

--------------------------------------------------------------------------------
第十七章 [高级进程间通信](https://en.wikipedia.org/wiki/Inter-process_communication 'inter-process communication')
================================================================================

## 17.1 引言

一种高级IPC - UNIX域套接字机制。

**这种形式的 
[IPC](https://en.wikipedia.org/wiki/Inter-process_communication 'inter-process communication') 
可以在同一计算机系统上运行的两个进程之间转送打开文件描述符。服务进程可以使用它们
的打开文件描述符与指定的名字相关联，同一系统上运行的客户进程可以使用这些名字与服
务器进程汇聚。**

[BACK TO TOP](#目录)

## 17.2 UNIX域套接字

**[UNIX 域套接字](https://en.wikipedia.org/wiki/Unix_domain_socket 'unix domain socket - wikipedia') 
用于在同一台计算机上运行的进程之间的通信。虽然因特网域套接字可用于同一目的，但 
UNIX 域套接字的效率更高。UNIX 域套接字仅仅复制数据，它们并不执行协议处理，不需要
添加或删除网络报头，无需计算校验和，不要产生顺序号，无需发送确认报文。**

UNIX 域套接字提供 **流和数据报** 两种接口。UNIX 域数据报服务是可的，既不会丢失报
文也不会传递出错。UNIX 域套接字就像是套接字和管道的混合。

一对相互连接的 UNIX 域套接字可以起到 **全双工管道** 的作用：两端对读和写开放。

### Figure-17.2.c

功能：使用 `socketpair` 函数来创建一个全双工的管道。

涉及头文件：
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[socketpair(2)](http://man7.org/linux/man-pages/man2/socketpair.2.html)

沉淀内容：学习理解使用 `socketpair`

```

 #include "apue.h"
 #include <sys/socket.h>
 
 /*
  * Returns a full-duplex pipe (a UNIX domain socket) with
  * the two file descriptors returned in fd[0] and fd[1].
  */
 int
 fd_pipe(int fd[2])
 {
     return(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
 }
```

[15.6.4](#xx) 节曾经提到 
[XSI](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_444 'XSI - opengroup') 
消息队列的使用存在一个问题，即不能将它们和 `poll` 或者 `select` 一起使用，这是因
为它们不能关联到文件描述符。然而，套接字是和文件描述符相关联的，消息到达时，可以
用套接字来通知。对每个消息队列使用一个线程。每个线程都会在 `msgrcv` 调用中阻塞。
当消息到达时，线程会把它写入一个 UNIX 域套接字的一端。 当 `poll` 指示套接字可以
读取数据时，应用程序就会使用套接字的另一端来接受这个消息。

### Figure-17.3.c

功能：通过消息队列和 UNIX 域套接字在进程间通信。

涉及头文件：
[poll.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/poll.h.html)，
[pthread.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)，
[sys/msg.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_msg.h.html)，
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

涉及函数：
[msgrcv(2)](http://man7.org/linux/man-pages/man2/msgrcv.2.html)，
[msgget(2)](http://man7.org/linux/man-pages/man2/msgget.2.html)，
[socketpair(2)](http://man7.org/linux/man-pages/man2/socketpair.2.html)，
[pthread_create(3)](http://man7.org/linux/man-pages/man3/pthread_create.3.html)

沉淀内容：练习使用 poll，消息队列，UNIX 域套接字。

```c

 #include "apue.h"
 #include <poll.h>
 #include <pthread.h>
 #include <sys/msg.h>
 #include <sys/socket.h>
 
 #define NQ      3       /* number of queues */
 #define MAXMSZ  512     /* maximum message size */
 #define KEY     0x123   /* key for first message queue */
 
 struct threadinfo {
     int     qid;
     int     fd;
 };
 
 struct mymesg {
     long    mtype;
     char    mtext[MAXMSZ];
 };
 
 void *
 helper(void *arg)
 {
     int                  n;
     struct mymesg        m;
     struct threadinfo   *tip = arg;
 
     for (;;) {
         memset(&m, 0, sizeof(m));
         if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)
             err_sys("msgrcv error");
         if (write(tip->fd, m.mtext, n) < 0)
             err_sys("write error");
     }
 }
 
 /* gcc apue.h apue_err.c figure-17.3.c -lpthread -o pollmsg */
 int
 main()
 {
     int                 i, n, err;
     int                 fd[2];
     int                 qid[NQ];
     struct pollfd       pfd[NQ];
     struct threadinfo   ti[NQ];
     pthread_t           tid[NQ];
     char                buf[MAXMSZ];
 
     for (i = 0; i < NQ; ++i) {
         if ((qid[i] = msgget((KEY + i), IPC_CREAT | 0666)) < 0)
             err_sys("msgget error");
 
         printf("queue ID %d is %d\n", i, qid[i]);
 
         if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
             err_sys("socketpair error");
         pfd[i].fd = fd[0];
         pfd[i].events = POLLIN;
         ti[i].qid = qid[i];
         ti[i].fd = fd[1];
         if ((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0)
             err_exit(err, "pthread_create error");
     }
 
     for (;;) {
         if (poll(pfd, NQ, -1) < 0)
             err_sys("poll error");
         for (i = 0; i < NQ; ++i) {
             if (pfd[i].revents & POLLIN) {
                 if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)
                     err_sys("read error");
                 buf[n] = 0;
                 printf("queue id %d, message %s\n", qid[i], buf);
             }
         }
     }
     
     exit(0);
 }
```

以上例子中各个技术点的用途：

|技术|用途|
|:--:|:--:|
|消息队列|用于在进程间通信。|
|UNIX 域套接字|解决消息队列不能一次使用多个的情况。（参见 [APUE 15.6.4](#xx)）|
|poll|使用多路转接I/O函数，处理多个文件描述符（UNIX 域套接字）。|
|线程|每个队列消费数据。|

注意：UNIX 域套接字在此处使用的是数据报（SOCK_DGRAM）套接字而不是流套接字。这样
做可以保持消息边界，以保证从套接字里一次只读取一条消息。

**这种技术可以（非直接地）在消息队列中运用 `poll` 或者 `select`。只要为每个队列
分配一个线程的开销以及每个消息额外复制两次（一次写入套接字，另一次从套接字里读取
出来）的开销是可接受的，这种技术就会是 XSI 消息队列的使用更加容易。**

### Figure-17.4.c

功能：[Figure-17.3.c](#figure-173c) 的测试程序

涉及头文件：
[sys/msg.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_msg.h.html)

涉及函数：
[msgsnd(2)](http://man7.org/linux/man-pages/man2/msgsnd.2.html)

沉淀内容：掌握消息队列的应用

```c

 #include "apue.h"
 #include <sys/msg.h>
 
 #define MAXMSZ  512
 
 struct mymesg {
     long    mtype;
     char    mtext[MAXMSZ];
 };
 
 /* gcc apue.h apue_err.c figure-17.4.c -o sendmsg */
 int
 main(int argc, char *argv[])
 {
     key_t           key;
     long            qid;
     size_t          nbytes;
     struct mymesg   m;
 
     if (argc != 3) {
         fprintf(stderr, "usage: sendmsg KEY message\n");
         exit(1);
     }
     key = strtol(argv[1], NULL, 0);
     if ((qid = msgget(key, 0)) < 0)
         err_sys("can't open queue key %s", argv[1]);
     memset(&m, 0, sizeof(m));
     strncpy(m.mtext, argv[2], MAXMSZ - 1);
     nbytes = strlen(m.mtext);
     m.mtype = 1;
     if (msgsnd(qid, &m, nbytes, 0) < 0)
         err_sys("can't send message");
     exit(0);
 }
```

获取已有的消息队列，并向指定的消息队列写入消息。

执行结果：

```sh

 # Figure-17.4.c
 [fanbin@localhost apue]$ gcc apue.h apue_err.c figure-17.3.c -lpthread -o pollmsg
 [fanbin@localhost apue]$ ./pollmsg &
 [1] 1071
 [fanbin@localhost apue]$ queue ID 0 is 0
 queue ID 1 is 32769
 queue ID 2 is 65538
 queue id 0, message hello, world
 queue id 32769, message just a test
 queue id 65538, message bye
 
 # Figure-17.3.c
 [fanbin@localhost apue]$ gcc apue.h apue_err.c figure-17.4.c -o sendmsg
 [fanbin@localhost apue]$ ./sendmsg 0x123 "hello, world"
 [fanbin@localhost apue]$ ./sendmsg 0x124 "just a test"
 [fanbin@localhost apue]$ ./sendmsg 0x125 "bye"
 [fanbin@localhost apue]$ 
```

## 命令UNIX域套接字

虽然 `socketpair` 函数能创建一个相互连接的套接字，但是每一个套接字都没有名字。这
意味着无关进程不能使用它们。

如果我们试图绑定同一地址时，该文件已经存在，那么 `bind` 请求会失败。当关闭套接字
时，并不自动删除该文件，所以必须确保在应用程序退出前，对该文件执行解除链接操作。

### Figure-17.5.c

功能：创建一个命名 UNIX 域套接字

涉及头文件：
[sys/socket.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)，
[sys/un.h](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_un.h.html)

涉及函数：
[offsetof(3)](http://man7.org/linux/man-pages/man3/offsetof.3.html)

沉淀内容：学会创建 UNIX 域套接字，了解该套接字需要手动解除链接。

```c

 #include "apue.h"
 #include <sys/socket.h>
 #include <sys/un.h>
 
 /* gcc apue.h apue_err.c figure-17.5.c */
 int
 main(void)
 {
     int                 fd, size;
     struct sockaddr_un  un;
 
     un.sun_family = AF_UNIX;
     strcpy(un.sun_path, "foo.socket");
     if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
         err_sys("socket failed");
     size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
     if (bind(fd, (struct sockaddr *)&un, size) < 0)
         err_sys("bind failed");
     printf("UNIX domain socket bound\n");
     exit(0);
 }
```

执行结果：

```sh

 [fanbin@localhost apue]$ ./a.out                        # 运行该程序
 UNIX domain socket bound
 [fanbin@localhost apue]$ ls -l foo.socket               # 查看套接字文件
 srwxrwxr-x. 1 fanbin fanbin 0 Mar 10 17:10 foo.socket
 [fanbin@localhost apue]$ ./a.out                        # 试图再次运行该程序
 bind failed: Address already in use
 [fanbin@localhost apue]$ rm foo.socket                  # 删除该套接字文件
 [fanbin@localhost apue]$ ./a.out                        # 第三次运行该程序
 UNIX domain socket bound
 [fanbin@localhost apue]$ 
```

获取结构中某一字段的偏移量使用 `offsetof`，定义如下：

```c

 #define offsetof(TYPE, MEMBER) ((int)&((TYPE *)0)->MEMBER)
```

[BACK TO TOP](#目录)

--------------------------------------------------------------------------------
附录B 其他源代码
================================================================================

## B1 本书使用的头文件

### apue.h

```c

 /*
  * Our own header, to be included before all standard system headers.
  */
 #ifndef _APUE_H
 #define _APUE_H
 
 #define _POSIX_C_SOURCE 200809L
 
 #if defined(SOLARIS)        /* Solaris 10 */
 #define _XOPEN_SOURCE 600
 #else
 #define _XOPEN_SOURCE 700
 #endif
 
 #include <sys/types.h>      /* some system still require this */
 #include <sys/stat.h>
 #include <sys/termios.h>    /* for winsize */
 
 #if defined(MACOS) || !defined(TIOCGWINSZ)
 #include <sys/ioctl.h>
 #endif
 
 #include <stdio.h>          /* for convenience */
 #include <stdlib.h>         /* for convenience */
 #include <stddef.h>         /* for offsetof */
 #include <string.h>         /* for convenience */
 #include <unistd.h>
 #include <signal.h>
 
 #define MAXLINE 4096 /* max line length */
 
 /*
  * Default file access permission for new files.
  */
 #define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
 
 /*
  * Default permissions for new directories.
  */
 #define DIR_MODE (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
 
 typedef void Sigfunc(int); /* for singnal handlers */
 
 #if defined(SIG_IGN) && !defined(SIG_ERR)
 #define SIG_ERR ((Sigfunc *)-1)
 #endif
 
 #define min(a, b)  ((a) < (b) ? (a) : (b))
 #define max(a, b)  ((a) > (b) ? (a) : (b))
 
 /*
  * Prototypes for our own functions.
  */
 //char    *path_alloc(int *);                                 /* Figure 2.16 */
 char    *path_alloc(size_t *);                                /* Figure 2.16 */
 long     open_max(void);                                      /* Figure 2.17 */
 
 int      set_cloexec(int);                                    /* Figure 13.9 */
 void     clr_fl(int, int);
 void     set_fl(int, int);
 
 void     pr_exit(int);                                        /* Figure 3.12 */
 
 void     pr_mask(const char *);                               /* Figure 10.14 */
 Sigfunc *signal_intr(int, Sigfunc *);                         /* Figure 10.19 */
 
 void     daemonize(const char *);                             /* Figure 13.1 */
 
 void     sleep_us(unsigned int);                              /* Exercise 14.5 */
 ssize_t  readn(int, void*, size_t);                           /* Figure 14.24 */
 ssize_t  writen(int, const void*, size_t);                    /* Figure 14.24 */
 
 int      fd_pipe(int *);                                      /* Figure 17.2 */
 int      recv_fd(int, ssize_t (*func)(int,
             const void *, size_t));                           /* Figure 17.14 */
 int      send_fd(int, int);                                   /* Figure 17.13 */
 int      send_err(int, int, const char *);                    /* Figure 17.12 */
 int      serv_listen(const char *);                           /* Figure 17.8 */
 int      serv_accept(int, uid_t *);                           /* Figure 17.9 */
 int      cli_conn(const char *);                              /* Figure 17.10 */
 int      buf_args(char *, int (*func)(int,
                   char **));                                  /* Figure 17.23 */
 
 int      tty_cbread(int);                                     /* Figure 17.20 */
 int      tty_raw(int);                                        /* Figure 17.20 */
 int      tty_reset(int);                                      /* Figure 17.20 */
 void     tty_atexit(void);                                    /* Figure 17.20 */
 struct termios *tty_termios(void);                            /* Figure 17.20 */
 
 int      ptym_open(char *, int);                              /* Figure 19.9 */
 int      ptys_open(char *);                                   /* Figure 19.9 */
 #ifdef TIOCGWINSZ
 pid_t    pty_fork(int *, char *, int, const struct termios *,
                   const struct winsize *);                    /* Figure 19.10 */
 #endif
 
 int      lock_reg(int, int, int, off_t, int, off_t);          /* Figure 14.5 */
 
 #define  read_lock(fd, offset, whence, len) \
              lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
 #define  readw_lock(fd, offset, whence, len) \
              lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
 #define  write_lock(fd, offset, whence, len) \
              lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
 #define  writew_lock(fd, offset, whence, len) \
              lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len)) 
 #define  un_lock(fd, offset, whence, len) \
              lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))
 
 pid_t lock_test(int, int, off_t, int, off_t);                 /* Figure 14.6 */
 
 #define  is_read_lockable(fd, offset, whence, len) \
              (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
 #define  is_write_lockable(fd, offset, whence, len) \
              (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)
 
 void     err_msg(const char *, ...); /* Appendix B */
 void     err_dump(const char *, ...) __attribute__((noreturn));
 void     err_quit(const char *, ...) __attribute__((noreturn));
 void     err_cont(int, const char *, ...);
 void     err_exit(int, const char *, ...) __attribute__((noreturn));
 void     err_ret(const char *, ...);
 void     err_sys(const char *, ...) __attribute__((noreturn));
 
 void     log_msg(const char *, ...); /* Appendix B */
 void     log_open(const char *, int, int);
 void     log_quit(const char *, ...) __attribute__((noreturn));
 void     log_ret(const char *, ...);
 void     log_sys(const char *, ...) __attribute__((noreturn));
 void     log_exit(int, const char *, ...) __attribute__((noreturn));
 
 void     TELL_WAIT(void); /* parent/child from Section 8.9 */
 void     TELL_PARENT(pid_t);
 void     TELL_CHILD(pid_t);
 void     WAIT_PARENT(void);
 void     WAIT_CHILD(void);
 
 #endif /* _APUE_H */
```

## B2 标准出错例程

### apue_err.c

```c

 #include "apue.h"
 #include <errno.h> /* for definition of errno */
 #include <stdarg.h> /* ISO C variable aruments */
 
 static void err_doit(int, int, const char *, va_list);
 
 /*
  * Nonfatal error related to a system call.
  * Print a message and return.
  */
 
 void
 err_ret(const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     err_doit(1, errno, fmt, ap);
     va_end(ap);
 }
 
 /*
  * Fatal error related to a system call.
  * Print a message and terminate.
  */
 void
 err_sys(const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     err_doit(1, errno, fmt, ap);
     va_end(ap);
     exit(1);
 }
 
 /*
  * Nonfatal error unrelated to a system call.
  * Error code passed as explict parameter.
  * Print a message and return.
  */
 void
 err_cont(int error, const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     err_doit(1, error, fmt, ap);
     va_end(ap);
 }
 
 /*
  * Fatal error unrelated to a system call.
  * Error code passed as explict parameter.
  * Print a message and terminate.
  */
 void
 err_exit(int error, const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     err_doit(1, error, fmt, ap);
     va_end(ap);
     exit(1);
 }
 
 /*
  * Fatal error related to a system call.
  * Print a message, dump core, and terminate.
  */
 void
 err_dump(const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     err_doit(1, errno, fmt, ap);
     va_end(ap);
     abort(); /* dump core and terminate */
     exit(1); /* shouldn't get here */
 }
 
 /*
  * Nonfatal error unrelated to a system call.
  * Print a message and return.
  */
 void
 err_msg(const char *fmt, ...)
 {
     va_list ap;
     
     va_start(ap, fmt);
     err_doit(0, 0, fmt, ap);
     va_end(ap);
 }
 
 /*
  * Fatal error unrelated to a system call.
  * Print a message and terminate.
  */
 void
 err_quit(const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     err_doit(0, 0, fmt, ap);
     va_end(ap);
     exit(1);
 }
 
 /*
  * Print a message and return to caller.
  * Caller specifies "errnoflag".
  */
 static void
 err_doit(int errnoflag, int error, const char *fmt, va_list ap)
 {
     char buf[MAXLINE];
     vsnprintf(buf, MAXLINE, fmt, ap);
     if (errnoflag) {
         snprintf(buf + strlen(buf), MAXLINE - strlen(buf), ": %s",
             strerror(error));
     }
     strcat(buf, "\n");
     fflush(stdout); /* in cast stdout and stderr are the same */
     fputs(buf, stderr);
     fflush(NULL); /* flushes all stdio output streams */
 }
 ```
 
 ### apue_log.c
 
 ```c
 /*
  * Error routines for programs that can run as a daemon.
  */
 
 #include "apue.h"
 #include <errno.h> /* for definition of errno */
 #include <stdarg.h> /* ISO C variable arguments */
 #include <syslog.h>
 
 static void log_doit(int, int, int, const char *, va_list ap);
 
 /*
  * Caller must define and set this: nonzero if 
  * interactive, zero if daemon
  */
 extern int log_to_stderr;
 
 /*
  * Initialize syslog(), if running as daemon.
  */
 void
 log_open(const char *ident, int option, int facility)
 {
     if (log_to_stderr == 0) {
         openlog(ident, option, facility);
     }
 }
 
 /*
  * Nonfatal error related to a system call.
  * Print a message with the system's errno value and return.
  */
 void
 log_ret(const char *fmt, ...)
 {
     va_list ap;
     
     va_start(ap, fmt);
     log_doit(1, errno, LOG_ERR, fmt, ap);
     va_end(ap);
 }
 
 /*
  * Fatal error related to a system call.
  * Print a message and terminate.
  */
 void
 log_sys(const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     log_doit(1, errno, LOG_ERR, fmt, ap);
     va_end(ap);
     exit(2);
 }
 
 /*
  * Nonfatal error unrelated to a system call.
  * Print a message and return.
  */
 void 
 log_msg(const char *fmt, ...)
 {
     va_list ap;
     
     va_start(ap, fmt);
     log_doit(0, 0, LOG_ERR, fmt, ap);
     va_end(ap);
 }
 
 /*
  * Fatal error unrelated to a system call.
  * Print a message and terminate.
  */
 void
 log_quit(const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     log_doit(0, 0, LOG_ERR, fmt, ap);
     va_end(ap);
     exit(2);
 }
 
 /*
  * Fatal error related to a systemcall.
  * Error number passed as an explicit parameter.
  * Print a message and terminate.
  */
 void
 log_exit(int error, const char *fmt, ...)
 {
     va_list ap;
 
     va_start(ap, fmt);
     log_doit(1, error, LOG_ERR, fmt, ap);
     va_end(ap);
     exit(2);
 }
 
 /*
  * Print a message and return to caller.
  * Caller specifies "errnoflag" and "priority".
  */
 static void
 log_doit(int errnoflag, int error, int priority, const char *fmt, va_list ap)
 {
     int  errno_save;
     char buf[MAXLINE];
 
     errno_save = errno; /* value caller might want printed */
     vsnprintf(buf, MAXLINE, fmt, ap);
     if (errnoflag) {
         snprintf(buf + strlen(buf), MAXLINE - strlen(buf), ": %s",
             strerror(errno_save));
     }
     strcat(buf, "\n");
     if (log_to_stderr) {
         fflush(stdout);
         fputs(buf, stderr);
         fflush(stderr);
     } else {
         syslog(priority, buf);
     }
 }
```

[BACK TO TOP](#目录)

--------------------------------------------------------------------------------
参考资料
================================================================================

https://en.wikipedia.org/wiki/Main_Page

https://www.kernel.org/doc/man-pages/

http://pubs.opengroup.org/onlinepubs/9699919799/

http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/contents.html

[UNIX环境高级编程（第三版）](http://www.amazon.cn/UNIX%E7%8E%AF%E5%A2%83%E9%AB%98%E7%BA%A7%E7%BC%96%E7%A8%8B-%E5%8F%B2%E8%92%82%E6%96%87%E6%96%AF/dp/B00KMR129E/ref=sr_1_1?ie=UTF8&qid=1456286853&sr=8-1&keywords=unix%E7%8E%AF%E5%A2%83%E9%AB%98%E7%BA%A7%E7%BC%96%E7%A8%8B+%E7%AC%AC3%E7%89%88#nav-belt)
