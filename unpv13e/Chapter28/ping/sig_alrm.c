#include "ping.h"
#include <unistd.h>

void sig_alrm(int signo)
{
    (*pr->fsend)();
    alarm(1);
}
