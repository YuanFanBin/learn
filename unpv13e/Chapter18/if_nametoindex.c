#include "../Chapter17/unpifi.h"
#include "unproute.h"
#include <string.h>

unsigned int if_nametoindex(const char *name)
{
    unsigned int        idx, namelen;
    char                *buf, *next, *lim;
    size_t               len;
    struct if_msghdr    *ifm;
    struct sockaddr     *sa, *rti_info[RTAX_MAX];
    struct sockaddr_dl  *sdl;

    // 获取接口列表
    if ((buf = net_rt_iflist(0, 0, &len)) == NULL) {
        return(0);
    }

    // 查找符合给定名称的接口，并返回其索引
    namelen = strlen(name);
    lim = buf + len;
    for (next = buf; next < lim; next += ifm->ifm_msglen) {
        ifm = (struct if_msghdr *) next;
        if (ifm->ifm_type == RTM_IFINFO) {
            sa = (struct sockaddr *) (ifm + 1);
            get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
            if ((sa = rti_info[RTAX_IFP]) != NULL) {
                if (sa->sa_family == AF_LINK) {
                    sdl = (struct sockaddr_d *) sa;
                    if (sdl->sdl_nlen == namelen && strncmp(&sdl->sdl_data[0], name, sdl->sdl_nlen) == 0) {
                        idx = sdl->sdl_index; // save before free()
                        free(buf);
                        return(idx);
                    }
                }
            }
        }
    }
    free(buf);
    return(0);  // no match for name
}
