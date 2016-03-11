#include <unistd.h>
#include <time.h>
#include <sys/select.h>

unsigned
sleep(unsigned seconds)
{
    int             n;
    unsigned        slept;
    time_t          start, end;
    struct timeval  tv;

    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    time(&start);
    n = select(0, NULL, NULL, NULL, &tv);
    if (n == 0)
        return(0);
    time(&end);
    slept = end - start;
    if (slept >= seconds)
        return(0);
    return(seconds - slept);
}
/* 
 * 使用select实现线程安全的sleep函数。并不使用任何未经保护的全局或
 * 静态数据，并只调用其他线程安全的函数。
 */
