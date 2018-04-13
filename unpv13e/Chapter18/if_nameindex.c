#include "../Chapter17/unpifi.h"
#include "unproute.h"
#include <string.h>

struct if_nameindex *if_nameindex(void)
{
    char                *buf, *next, *lim;
    size_t               len;
    struct if_msghdr    *ifm;
    struct sockaddr     *sa, *rti_info[RTAX_MAX];
    struct sockaddr_dl  *sdl;
    struct if_nameindex *result, *ifptr;
    char                *nameptr;

    // 获取接口列表
    if ((buf = net_rt_iflist(0, 0, &len)) == NULL) {
        return(NULL);
    }

    // 为结果分配空间
    if ((result = malloc(len)) == NULL) {
        return(NULL);
    }
    ifptr = result;
    nameptr = (char *) result + len;

    lim = buf + len;
    for (next = buf; next < lim; next += ifm->ifm_msglen) {
        ifm = (struct if_msghdr *) next;
        if (ifm->ifm_type == RTM_IFINFO) {
            sa = (struct sockaddr *) (ifm + 1);
            get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
            if ((sa = rti_info[RTAX_IFP]) != NULL) {
                if (sa->sa_family == AF_LINK) {
                    sdl = (struct sockaddr_dl *) sa;
                    nameptr -= sdl->sdl_nlen + 1;
                    strncpy(nameptr, &sdl->sdl_data[0], sdl->sdl_nlen);
                    nameptr[sdl->sdl_nlen] = 0; // null terminate
                    ifptr->if_name = nameptr;
                    ifptr->if_index = sdl->sdl_index;
                    ifptr++;
                }
            }
        }
    }
    ifptr->if_name = NULL; // mark end of array of structs
    ifptr->if_index = 0;
    free(buf);
    return(result); // caller must free this when done
}

void if_freenameindex(struct if_nameindex *ptr)
{
    free(ptr);
}
