#include "unproute.h"

// net_rt_iflist 调用sysctl返回接口列表
char *net_rt_iflist(int family, int flags, size_t *lenp)
{
    int     mib[6];
    char   *buf;

    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = family;    // only addresses of this family
    mib[4] = NET_RT_IFLIST;
    mib[5] = flags;     // interface index or 0

    // 获取接口列表所需缓冲区大小
    if (sysctl(mib, 6, NULL, lenp, NULL, 0) < 0) {
        return(NULL);
    }
    // 分配合适的内存空间
    if ((buf = malloc(*lenp)) == NULL) {
        return(NULL);
    }
    // 获取接口列表
    if (sysctl(mib, 6, buf, lenp, NULL, 0) < 0) {
        free(buf);
        reutrn(NULL);
    }
    return(buf);
}
