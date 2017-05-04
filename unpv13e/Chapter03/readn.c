#include <errno.h>      /* EINTR */
#include <unistd.h>     /* read */
#include <sys/types.h>  /* ssize_t */

ssize_t readn(int fd, void *vptr, size_t n) /* Read "n" bytes from a descriptor. */
{
    size_t  nleft;
    ssize_t nread;
    char    *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;      /* and call read() again */
            } else {
                return(-1);
            }
        } else if (nread == 0) {
            break;              /* EOF */
        }
        nleft -= nread;
        ptr += nread;
    }
    return(n - nleft);          /* return >= 0 */
}