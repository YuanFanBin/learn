/* man 7 aio(http://man7.org/linux/man-pages/man7/aio.7.html) */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <aio.h>
#include <signal.h>

#define BUF_SIZE    20      /* Size of buffers for read operations */

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define errMsg(msg) do { perror(msg); } while (0)

struct ioRequest {          /* Application-defined structure for tracking I/O requests */
    int           reqNum;
    int           status;
    struct aiocb *aiocbp;
};

/* On delivery of SIGQUIT, we attempt to cancel all outstanding I/O requests */
static volatile sig_atomic_t gotSIGQUIT = 0;

static void     /* Handler for SIGQUIT */
quitHandler(int sig)
{
    gotSIGQUIT = 1;
}

#define IO_SIGNAL   SIGUSR1     /* Signal used to notify I/O completion */

static void     /* Handler for I/O completion signal */
aioSigHandler(int sig, siginfo_t *si, void *ucontext)
{
    if (si->si_code == SI_ASYNCIO) {
        write(STDOUT_FILENO, "I/O completion signal received\n", 31);

        /* 
         * The corresponding ioRequest structure would be available as
         *     struct ioRequest *ioReq = si->si_value.sival_ptr;
         * and the file descriptor would then be available via
         *     ioReq->aiocbp->aio_fildes
         */
    }
}

/* gcc section-14.5.c -lrt */
int
main(int argc, char *argv[])
{
    struct ioRequest *ioList;
    struct aiocb     *aiocbList;
    struct sigaction  sa;
    int               s, j;
    int               numReqs;  /* Total number of queued I/O requests */
    int               openReqs; /* Number of I/O request still in progress */

    if (argc < 2) {
        fprintf(stderr, "Usage %s <pathname> <pathname>...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numReqs = argc - 1;

    /* Allocate our arrays */

    ioList = calloc(numReqs, sizeof(struct ioRequest));
    if (ioList == NULL)
        errExit("calloc");

    aiocbList = calloc(numReqs, sizeof(struct aiocb));
    if (aiocbList == NULL)
        errExit("calloc");

    /* Establish handlers for SIGQUIT and the I/O completion signal */

    sa.sa_flags = SA_RESTART;   /* 自动重启动 */
    sigemptyset(&sa.sa_mask);
    
    sa.sa_handler = quitHandler;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)    /* 捕获 SIGQUIT 中断信号 */
        errExit("sigaction");

    /* 异步读操作完成时，会以此信号作为通知 */
    sa.sa_flags = SA_RESTART | SA_SIGINFO;  /* 自动重启动&使用sa_sigaction作为信号处理程序 */
    sa.sa_sigaction = aioSigHandler;
    if (sigaction(IO_SIGNAL, &sa, NULL) == -1)  /* 捕获 SIGUSR1 信号 */
        errExit("sigaction");

    /*
     * Open each file specified on the command line, and queue
     * a read request on the resulting file descriptor
     */

    for (j = 0; j < numReqs; j++) {
        ioList[j].reqNum = j;
        ioList[j].status = EINPROGRESS;
        ioList[j].aiocbp = &aiocbList[j];

        /* 打开参数所指定的文件 */
        ioList[j].aiocbp->aio_fildes = open(argv[j + 1], O_RDONLY);
        if (ioList[j].aiocbp->aio_fildes == -1)
            errExit("open");
        printf("opened %s on descriptor %d\n", argv[j + 1], ioList[j].aiocbp->aio_fildes);

        /* 设置异步I/O的buf大小 */
        ioList[j].aiocbp->aio_buf = malloc(BUF_SIZE);
        if (ioList[j].aiocbp->aio_buf == NULL)
            errExit("malloc");

        /* 其他相关设置 */
        ioList[j].aiocbp->aio_nbytes = BUF_SIZE;
        ioList[j].aiocbp->aio_reqprio = 0;
        ioList[j].aiocbp->aio_offset = 0;
        /* 设置在异步I/O事件完成后如何通知应用程序 */
        // 使用sigev_signo所指定的信号通知
        ioList[j].aiocbp->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        ioList[j].aiocbp->aio_sigevent.sigev_signo = IO_SIGNAL;
        ioList[j].aiocbp->aio_sigevent.sigev_value.sival_ptr = &ioList[j];

        /* 启动异步读操作（并未开始实际的读操作）*/
        /*
         * 当函数返回时，只是把请求的读操作放入了等待处理的队列中了
         */
        s = aio_read(ioList[j].aiocbp);
        if (s == -1)
            errExit("aio_read");
    }

    openReqs = numReqs;

    /* Loop, monitoring status of I/O requests */

    while (openReqs > 0) {
        sleep(3);       /* Delay between each monitoring step */

        if (gotSIGQUIT) {
            /*
             * On receipt of SIGQUIT, attempt to cancel each of the
             * outstanding I/O requests, and display status returned
             * from the cancellation requests
             */
            printf("got SIGQUIT; canceling I/O requests: \n");

            /* 发生 SIGQUIT 中断时，取消所有正在进行的异步IO操作 */
            for (j = 0; j < numReqs; j++) {
                if (ioList[j].status == EINPROGRESS) {
                    printf("    Request %d on descriptor %d:", j, 
                            ioList[j].aiocbp->aio_fildes);
                    s = aio_cancel(ioList[j].aiocbp->aio_fildes,
                            ioList[j].aiocbp);
                    if (s == AIO_CANCELED)
                        printf("I/O canceled\n");
                    else if (s == AIO_NOTCANCELED)
                        printf("I/O not canceled");
                    else if (s == AIO_ALLDONE)
                        printf("I/O all done\n");
                    else
                        errMsg("aio_cancel");
                }
            }

            gotSIGQUIT = 0;
        }

        /* Check the status of each I/O request that is still in progress */

        /* 获取当前异步IO的状态 */
        printf("aio_error():\n");
        for (j = 0; j < numReqs; j++) {
            if (ioList[j].status == EINPROGRESS) {
                printf("    for request %d (descriptor %d):", j,
                        ioList[j].aiocbp->aio_fildes);
                ioList[j].status = aio_error(ioList[j].aiocbp);

                switch(ioList[j].status) {
                case 0:
                    printf("I/O successed\n");
                    break;
                case EINPROGRESS:
                    printf("In progress\n");
                    break;
                case ECANCELED:
                    printf("Canceled\n");
                    break;
                default:
                    errMsg("aio_error");
                    break;
                }

                if (ioList[j].status != EINPROGRESS)
                    openReqs--;
            }
        }
    }

    printf("All I/O requests completed\n");

    /* Check status return of all I/O requests */

    printf("aio_return():\n");
    for (j = 0; j < numReqs; j++) {
        ssize_t s;

        s = aio_return(ioList[j].aiocbp);
        printf("    for request %d (descriptor %d): %zd\n", j,
                ioList[j].aiocbp->aio_fildes, s);
    }

    exit(EXIT_SUCCESS);
}

/*
 * $ ./a.out /dev/stdin /dev/stdin
 * opened /dev/stdin on descriptor 3
 * opened /dev/stdin on descriptor 4
 * aio_error():
 *     for request 0 (descriptor 3): In progress
 *     for request 1 (descriptor 4): In progress
 * abc
 * I/O completion signal received
 * aio_error():
 *     for request 0 (descriptor 3): I/O succeeded
 *     for request 1 (descriptor 4): In progress
 * aio_error():
 *     for request 1 (descriptor 4): In progress
 * x
 * I/O completion signal received
 * aio_error():
 *     for request 1 (descriptor 4): I/O successed
 * All I/O requests completed
 * aio_return():
 *     for request 0 (descriptor 3): 4
 *     for request 1 (descriptor 4): 2
 */
