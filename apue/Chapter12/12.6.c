#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define MAXSTRINGSZ 4096

static pthread_key_t  key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
pthread_mutex_t       env_mutex = PHTREAD_MUTEX_INITIALIZER;

extern char **environ;

static void
thread_init(void)
{
    pthread_key_create(&key, free); /* 线程key非空，free析构函数 */
}

/* 线程安全：调用者不用提供自己的缓冲区，但不是异步信号安全的 */
/* 
 * 对信号处理程序而言，即时使用递归的互斥量，这个版本的getenv也不可能是
 * 可重入的，因为它调用了malloc，而malloc函数本身并不是异步信号安全的。
 */
/* 可重入函数：P162, 10.6 */
char *
getenv(const char *name)
{
    int     i, len;
    char   *envbuf;

    pthread_once(&init_done, thread_init);
    pthread_mutex_lock(&env_mutex);
    envbuf = (char *)pthread_getspecific(key);
    if (envbuf == NULL) {
        envbuf = malloc(MAXSTRINGSZ);
        if (envbuf == NULL) {
            pthread_mutex_unlock(&env_mutex);
            return(NULL);
        }
        pthread_setspecific(key, envbuf); /* 键与缓存关联 */
    }
    len = strlen(name);
    for (i = 0; environ[i] != NULL; i++) {
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            strncpy(envbuf, environ[i][len + 1], MAXSTRINGSZ - 1);
            pthread_mutex_unlock(&env_mutex);
            return(envbuf);
        }
    }
    pthread_mutex_unlock(&env_mutex);
    return(NULL);
}

/* 关于【可重入】&【线程安全】的文章
 * 1.http://www.cnblogs.com/zhaoyl/archive/2012/10/03/2711018.html
 * 2.http://blog.csdn.net/feiyinzilgd/article/details/5811157
 */
