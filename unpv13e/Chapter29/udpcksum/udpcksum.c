#include "udpcksum.h"
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

static sigjmp_buf   jmpbuf;
static int          canjump;

void sig_alrm(int signo)
{
    if (canjump == 0) {
        return;
    }
    siglongjmp(jmpbuf, 1);
}

void test_udp(void)
{
    // volatile变量：希望从信号处理函数 siglongjmp 到本函数之后保持它们的值不变
    volatile int     nsent = 0, timeout = 3;
    struct udpiphdr *ui;

    signal(SIGALRM, sig_alrm);

    if (sigsetjmp(jmpbuf, 1)) {
        if (nsent >= 3) {
            err_quit("no response");
        }
        printf("timeout\n");
        timeout *= 2;   // exponential backoff: 3, 6, 12
    }
    canjump = 1;    // siglongjmp is now OK

    send_dns_query();
    nsent++;

    alarm(timeout);
    ui = udp_read();
    canjump = 0;
    alarm(0);

    if (ui->ui_sum == 0) {
        printf("UDP checksums off\n");
    } else {
        printf("UDP checksums on\n");
    }
    if (verbose) {
        printf("received UDP checksum = %x\n", ntohs(ui->ui_sum));
    }
}
