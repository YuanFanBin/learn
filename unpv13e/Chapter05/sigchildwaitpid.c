#include <stdio.h>      /* printf */
#include <sys/wait.h>   /* waitpid */

void sig_child(int signo)
{
    pid_t   pid;
    int     stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
    return;
}
