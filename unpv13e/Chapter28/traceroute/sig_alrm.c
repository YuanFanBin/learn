#include "trace.h"

int gotalarm;

// 产生中断，从而 recvfrom 返回 EINTR
void sig_alrm(int signo)
{
    gotalarm = 1; // set flag to note that alarm occurred
}
