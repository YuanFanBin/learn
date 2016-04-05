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
    return(fcntl(fd, F_SETFL, &fl));
}

/* or
 * #define lockfile(fd) write_lock((fd), 0, SEEK_SET, 0)
 */