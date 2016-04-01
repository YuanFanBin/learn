#include <limit.h>
#include <string.h>

#define MAXSTRINGSZ 4096

static char envbuf[MAXSTRINGSC];

extern char **environ;

/* 不可重入版本，线程返回的字符串都存储在同一个静态缓冲区 */
char *
getenv(const char *name)
{
    int i, len;
    len = strlen(name);
    for (i = 0; environ[i] != NULL; i++) {
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            strncpy(envbuf, &environ[i][len+1], MAXSTRINGSC - 1);
            return(envbuf);
        }
    }
    return(NULL);
}



/* 线程安全：一个函数在相同的时间点可以被多个线程安全地调用。 */

/* 
 * 如果一个函数对多个线程来说是可重入的，就说这个函数就是线程安全的。
 * 但这并不能说明对信号处理程序来说该函数也是可重入的。/
 */

/* 
 * 异步信号安全：函数对异步信号处理程序的重入是安全的。
 * 图10-4中的函数是异步信号安全函数。 
 */
