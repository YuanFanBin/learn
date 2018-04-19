#include "unprtt.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

int rtt_d_flag = 0;     // debug flag; can be set by caller

// Calculate the RTO value based on current estimators:
//  smoothed RTT plus four times the deviation
#define RTT_RTOCALC(ptr) ((ptr)->rtt_srtt + (4.0 * (ptr)->rtt_rttvar))

// 确保RTO在定义的上下界之间
static float rtt_minmax(float rto)
{
    if (rto < RTT_RXTMIN) {
        rto = RTT_RXTMIN;
    } else if (rto > RTT_RXTMAX) {
        rto = RTT_RXTMAX;
    }
    return(rto);
}

void rtt_init(struct rtt_info *ptr)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    ptr->rtt_base = tv.tv_sec;  // # sec since 1/1/1970 at start
    ptr->rtt_rtt = 0;
    ptr->rtt_srtt = 0;
    ptr->rtt_rttvar = 0.75;
    ptr->rtt_rto = rtt_minmax(RTT_RTOCALC(ptr));
    // first RTO at (srtt * (4 * rttvar)) = 3 seconds
}

// 返回当前时间戳
uint32_t rtt_ts(struct rtt_info *ptr)
{
    uint32_t        ts;
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    ts = ((tv.tv_sec - ptr->rtt_base) * 1000) + (tv.tv_usec / 1000);
    return(ts);
}

// 把重传计数器设置为0
void rtt_newpack(struct rtt_info *ptr)
{
    ptr->rtt_nrexmt = 0;
}

// 以秒为单位返回当前RTO
int rtt_start(struct rtt_info *ptr)
{
    return((int) (ptr->rtt_rto + 0.5)); // round float to int
    // return value can be used as: alarm(rtt_start(&foo))
}

// 更新RTT估算因子并计算新的RTO
void rtt_stop(struct rtt_info *ptr, uint32_t ms)
{
    double delta;

    ptr->rtt_rtt = ms / 1000.0; // measured RTT in seconds

    // update our estimators of RTT and mean deviation of RTT.
    // see Jacobson's SIGCOMM '88 paper, Appendix A, for the details.
    // we use floating poing here for simplicity.
    delta = ptr->rtt_rtt - ptr->rtt_srtt;
    if (delta < 0.0) {
        delta = -delta;     // |delta|
    }
    ptr->rtt_rttvar += (delta - ptr->rtt_rttvar) / 4;   // h = 1/4
    ptr->rtt_rto = rtt_minmax(RTT_RTOCALC(ptr));
}

// 在重传定时器期满时调用
int rtt_timeout(struct rtt_info *ptr)
{
    ptr->rtt_rto *= 2;  // next RTO
    if (++ptr->rtt_nrexmt > RTT_MAXNREXMT) {
        return(-1);     // time to give up for this packet
    }
    return(0);
}
