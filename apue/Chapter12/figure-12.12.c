#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

extern char **environ;

pthread_mutex_t env_mutex;

static pthread_once_t init_done = PTHREAD_ONCE_INIT; 

static void
thread_init(void)
{
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); /* 递归锁 */
    pthread_mutex_init(&env_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

/* 可重入版本：调用者提供自己的缓冲区 */
int
getenv_t(const char *name, char *buf, int buflen)
{
    int i, len, olen;

    /* 12.6 pthread_once */
    pthread_once(&init_done, thread_init); /* thread_init 仅会调用1次 */
    len = strlen(name);
    pthread_mutex_lock(&env_mutex);
    for (i = 0; environ[i] != NULL; i++) {
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            olen = strlen(&environ[i][len + 1]);
            if (olen >= buflen) {
                pthread_mutex_unlock(&env_mutex);
                return(ENOSPC);
            }
            strcpy(buf, &environ[i][len + 1]);
            pthread_mutex_unlock(&env_mutex);
            return(0);
        }
    }
    pthread_mutex_unlock(&env_mutex);
    return(ENOENT);
}

/* pthread_once: 本函数使用初值为PTHREAD_ONCE_INIT的once_control变量保证
 * init_routine()函数在本进程执行序列中仅执行一次。*/
/* 参考资料：http://blog.csdn.net/sjin_1314/article/details/10934239 */
