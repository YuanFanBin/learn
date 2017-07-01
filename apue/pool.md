# 进程间通信&进程池

标签（空格分隔）： 未分类

---
TODO

- [ ] 1. 进程池 & 线程池 & 连接池 & 内存池
- [ ] 2. 进程间通信 APUE C17 (40%)

1. 进程池（学习过程）
 了解 [进程池](http://www.tuicool.com/articles/BBBfYnR)，然后了解 [进程间通信方式](https://en.wikipedia.org/wiki/Inter-process_communication 'inter-process communication') （[APUE 15.7 - 消息队列](#xx) & UNIX 域套接字 [APUE - 第十七章](#xx)），学习至 [17.3 唯一连接](#xx)，了解完 `serv_listen`, `serv_accept`, `cli_conn` 后学习 [APUE - 第十六章](#xx)（插曲 - [Figure-16.22.c](#xx) 连接池问题），在深入了解 [第十五章 进程间通信](#xx)，学至 [15.4](#xx) 后，需要再次理解 [第七章 进程环境](#xx) 

---
参考资料：
http://www.pureage.info/2015/03/28/ipc-of-nginx.html
http://www.tuicool.com/articles/BBBfYnR

消息队列实现：
http://blog.csdn.net/guosha/article/details/3874998

在此输入正文

```c
/* process_pool.h */
#ifndef PROCESS_POOL_H
#define PROCESS_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

class process
{
public:
    process() : m_pid (-1) {}

public:
    pid_t   m_pid;
    int     m_pipefd[2];
};

template<typename T>
class processpool
{
private:
    processpool(int listenfd, int process_number = 8);

public:
    static processpool<T> *create(int listenfd, int process_number = 8)
    {
        if (!m_instance) {
            m_instance = new processpool<T>(listenfd, process_number);
        }
        return m_instance;
    }
    ~processpool()
    {
        delete [] m_sub_process;
    }
    void run();

private:
    void setup_sig_pipe();
    void run_parent();
    void run_child();

private:
    static const int        MAX_PROCESS_NUMBER = 16;
    static const int        USER_PER_PROCESS = 65536;
    static const int        MAX_EVENT_NUMBER = 10000;
    int                     m_process_number;
    int                     m_idx;
    int                     m_epollfd;
    int                     m_listenfd;
    int                     m_stop;
    process                *m_sub_process;
    static processpool<T>  *m_instance;
};
template<typename T>
processpool<T> *processpool<T>::m_instance = NULL;

static int sig_pipefd[2];

static int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;

    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

static void addfd(int epollfd, int fd)
{
    epoll_event event;

    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

static void removefd(int epollfd, int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

static void sig_handler(int sig)
{
    int save_errno = errno;
    int msg = sig;
    send(sig_pipefd[1], (char*)&msg, 1, 0);
    errno = save_errno;
}

static void addsig(int sig, void(handler)(int), bool restart = true)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if (restart) {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

template<typename T>
processpool<T>::processpool(int listenfd, int process_number)
    : m_listenfd(listenfd), m_process_number(process_number), m_idx(-1), m_stop(false)
{
    assert((process_number > 0) && (process_number <= MAX_PROCESS_NUMBER));

    m_sub_process = new process[process_number];
    assert(m_sub_process);
    fprintf(stderr, "parent pid = %ld\n", (long)getpid());
    for (int i = 0; i < process_number; ++i) {
        int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, m_sub_process[i].m_pipefd);
        assert(ret == 0);

        m_sub_process[i].m_pid = fork();
        assert(m_sub_process[i].m_pid >= 0);
        if (m_sub_process[i].m_pid > 0) { /* 父进程 */
            close(m_sub_process[i].m_pipefd[1]);
            continue;
        } else {    /* 子进程 */
            fprintf(stderr, "child pid = %ld\n", (long)getpid());
            close(m_sub_process[i].m_pipefd[0]);
            m_idx = i;
            break;
        }
    }
}

template<typename T>
void processpool<T>::setup_sig_pipe()
{
    m_epollfd = epoll_create(5);
    assert(m_epollfd != -1);

    int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, sig_pipefd);
    assert(ret != -1);

    setnonblocking(sig_pipefd[1]);
    addfd(m_epollfd, sig_pipefd[0]);

    addsig(SIGCHLD, sig_handler);
    addsig(SIGTERM, sig_handler);
    addsig(SIGINT, sig_handler);
    addsig(SIGPIPE, SIG_IGN);
}

template<typename T>
void processpool<T>::run()
{
    if (m_idx != -1) {
        run_child();
        return;
    }
    run_parent();
}

template<typename T>
void processpool<T>::run_child()
{
    setup_sig_pipe();
    int pipefd = m_sub_process[m_idx].m_pipefd[1];
    addfd(m_epollfd, pipefd);

    epoll_event events[MAX_EVENT_NUMBER];
    T* users = new T[USER_PER_PROCESS];
    assert(users);
    int number = 0;
    int ret = -1;

    while (!m_stop) {
        number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }
        
        for (int i = 0; i < number; ++i) {
            int sockfd = events[i].data.fd;
            if ((sockfd == pipefd) && (events[i].events & EPOLLIN)) {
                int client = 0;
                ret = recv(sockfd, (char*)&client, sizeof(client), 0);
                if (((ret < 0) && (errno != EAGAIN)) || ret == 0) {
                    continue;

                } else {
                    struct sockaddr_in client_address;
                    socklen_t client_addrlength = sizeof(client_address);
                    int connfd = accept(m_listenfd, (struct sockaddr*)&client_address, &client_addrlength);
                    if (connfd < 0) {
                        printf("errno is %d\n", errno);
                        continue;
                    }
                    addfd(m_epollfd, connfd);
                    users[connfd].init(m_epollfd, connfd, client_address);
                }

            } else if ((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN)) {
                int sig;
                char signals[1024];
                ret = recv(sig_pipefd[0], signals, sizeof(signals), 0);
                if (ret <= 0) {
                    continue;
                } else {
                    for (int i = 0; i < ret; ++i) {
                        switch(signals[i]) {
                        case SIGCHLD:
                            pid_t pid;
                            int stat;
                            while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                                continue;
                            }
                            break;
                        case SIGTERM:
                        case SIGINT:
                            m_stop = true;
                            break;
                        default:
                            break;
                        }
                    }
                }

            } else if (events[i].events & EPOLLIN) {
                users[sockfd].process();
            } else {
                continue;
            }
        }
    }

    delete [] users;
    users = NULL;
    close(pipefd);
    close(m_epollfd);
}

template<typename T>
void processpool<T>::run_parent()
{
    setup_sig_pipe();
    addfd(m_epollfd, m_listenfd);

    epoll_event events[MAX_EVENT_NUMBER];
    int sub_process_counter = 0;
    int new_conn = 1;
    int number = 0;
    int ret = -1;

    while (!m_stop) {
        number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }

        for (int i = 0; i < number; ++i) {
            int sockfd = events[i].data.fd;
            if (sockfd == m_listenfd) {
                int i = sub_process_counter;
                do {
                    if (m_sub_process[i].m_pid != -1) {
                        break;
                    }
                    i = (i + 1) % m_process_number;
                } while (i != sub_process_counter);

                if (m_sub_process[i].m_pid == -1) {
                    m_stop = true;
                    break;
                }
                sub_process_counter = (i + 1) % m_process_number;
                send(m_sub_process[i].m_pipefd[0], (char *)&new_conn, sizeof(new_conn), 0);
                printf("send request to child %d\n", i);

            } else if ((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN)) {
                int sig;
                char signals[1024];
                ret = recv(sig_pipefd[0], signals, sizeof(signals), 0);
                if (ret <= 0) {
                    continue;
                } else {
                    for (int i = 0; i < ret; ++i) {
                        switch(signals[i]) {
                        case SIGCHLD:
                            pid_t pid;
                            int stat;
                            while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                                for (int i = 0; i < m_process_number; ++i) {
                                    if (m_sub_process[i].m_pid == pid) {
                                        printf("child %d join\n", i);
                                        close(m_sub_process[i].m_pipefd[0]);
                                        m_sub_process[i].m_pid = -1;
                                    }
                                }
                            }
                            m_stop = true;
                            for (int i = 0; i < m_process_number; ++i) {
                                if (m_sub_process[i].m_pid != -1) {
                                    m_stop = false;
                                }
                            }
                            break;
                        case SIGTERM:
                        case SIGINT:
                            printf("kill all the child now\n");
                            for (int i = 0; i < m_process_number; ++i) {
                                int pid = m_sub_process[i].m_pid;
                                if (pid != -1) {
                                    kill(pid, SIGTERM);
                                }
                            }
                            break;
                        default:
                            break;
                        }
                    }
                }
            } else {
                continue;
            }
        }
    }
    close(m_epollfd);
}

#endif

```

```c
/* process_poll.cpp */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "process_pool.h"

class cgi_conn
{
public:
    cgi_conn(){}
    ~cgi_conn(){}
    void init(int epollfd, int sockfd, const sockaddr_in &client_addr)
    {
        m_epollfd = epollfd;
        m_sockfd = sockfd;
        m_address = client_addr;
        memset(m_buf, '\0', BUFFER_SIZE);
        m_read_idx = 0;
    }

    void process()
    {
        int idx = 0;
        int ret = -1;
        while (true) {
            idx = m_read_idx;
            ret = recv(m_sockfd, m_buf + idx, BUFFER_SIZE - 1 - idx, 0);
            if (ret < 0) {
                if (errno != EAGAIN) {
                    removefd(m_epollfd, m_sockfd);
                }
                break;
            } else if (ret == 0) {
                removefd(m_epollfd, m_sockfd);
                break;
            } else {
                m_read_idx += ret;
                printf("user content is %s\n", m_buf);
                for (; idx < m_read_idx; ++idx) {
                    if ((idx >= 1) && (m_buf[idx - 1] == '\r') && (m_buf[idx] == '\n')) {
                        break;
                    }
                }
                if (idx == m_read_idx) {
                    continue;
                }
                m_buf[idx - 1] = '\0';

                char *file_name = m_buf;
                if (access(file_name, F_OK) == -1) {
                    removefd(m_epollfd, m_sockfd);
                    break;
                }
                ret = fork();
                if (ret == -1) {
                    removefd(m_epollfd, m_sockfd);
                    break;
                } else if (ret > 0) {
                    removefd(m_epollfd, m_sockfd);
                    break;
                } else {
                    close(STDOUT_FILENO);
                    dup(m_sockfd);
                    execl(m_buf, m_buf, 0);
                    exit(0);
                }
            }
        }
    }

private:
    static const int BUFFER_SIZE = 1024;
    static int m_epollfd;
    int m_sockfd;
    sockaddr_in m_address;
    char m_buf[BUFFER_SIZE];
    int m_read_idx;
};

int cgi_conn::m_epollfd = -1;

int main(int argc, char *argv[])
{
    if (argc <= 2) {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);
fprintf(stderr, "FFFFFFFFFFFFFFK-0\n");
    processpool<cgi_conn> *pool = processpool<cgi_conn>::create(listenfd);
    if (pool) {
        pool->run();
        delete pool;
    }
    close(listenfd);
    return 0;
}

```
