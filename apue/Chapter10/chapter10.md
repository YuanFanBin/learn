10.9
#include <signal.h>
int kill(pid_t pid, int signo);
POSIX.1 将信号0定义为空信号。如果signo参数是0，则kill仍执行正常的错误检查，
但不发送信号。这常常用来确定一个特定进程是否仍然存在。
