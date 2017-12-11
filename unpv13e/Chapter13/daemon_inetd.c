#include <syslog.h>
#include "../lib/error.h"

extern int daemon_proc; // default in error.c

void daemon_inetd(const char *pname, int facility)
{
    daemon_proc = 1;
    openlog(pname, LOG_PID, facility);
}
