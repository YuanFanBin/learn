#ifndef __unpicmp_h
#define __unpicmp_h

#include <sys/socket.h>
#include <sys/types.h>

#define ICMPD_PATH "/tmp/icmpd" // server's well-known pathname

struct icmpd_err {
    int icmpd_errno;                    // EHOSTUNREACH, EMSGSIZE, ECONNREFUSED
    char icmpd_type;                    // actual ICMPv[46] type
    char icmpd_code;                    // actual ICMPv[46] code
    socklen_t icmpd_len;                // length of sockaddr{} that follows
    struct sockaddr_storage icmpd_dest; // sockaddr_storage handles any size
};

// icmpd_errno  | ICMPv4错误               | ICMPv6错误
// ------------ | ------------------------ | ------------------------
// ECONNREFUSED | 端口不可达               | 端口不可达
// EMSGSIZE     | 需分片但DF位已设置       | 分组过大
// EHOSTUNREACH | 超时                     | 超时
// EHOSTUNREACH | 源熄灭                   |
// EHOSTUNREACH | 所有其他目的地不可达代码 | 所有其他目的地不可达代码

#endif // __unpicmp_h
