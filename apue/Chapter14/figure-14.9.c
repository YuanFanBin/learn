#include <unistd.h>
#include <fcntl.h>

/* figure-13.6.c */
int
lockfile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    /* 
     * 这里写错了，导致一直没能查出守护进程创建不正确的原因，
     * 总是能创建多个相同副本的进程 
     */
    //return(fcntl(fd, F_SETFL, &fl));
    return(fcntl(fd, F_SETLK, &fl));
}

/* or
 * #define lockfile(fd) write_lock((fd), 0, SEEK_SET, 0)
 */
