#ifndef __sntp_h
#define __sntp_h

#include "../../Chapter17/unpifi.h"
#include "ntp.h"
#include <sys/time.h>
#include <sys/types.h>

#define MAXLINE     4096    // max text line length

void sntp_proc(char *, ssize_t, struct timeval *);

#endif  // __sntp_h
