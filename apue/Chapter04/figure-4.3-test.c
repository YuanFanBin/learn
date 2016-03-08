#include "apue.h"

#define print(x) printf("%-15s: %d\n", #x, x)

int
main(int argc, char *argv[])
{
    struct stat     buf;
    if (lstat(argv[1], &buf) < 0) {
        err_ret("lstat error");
    }
    /* man lstat */
    print(buf.st_dev);
    print(buf.st_ino);
    print(buf.st_mode);
    print(buf.st_nlink);
    print(buf.st_uid);
    print(buf.st_gid);
    print(buf.st_rdev);
    print(buf.st_size);
    print(buf.st_blksize);
    print(buf.st_blocks);
    print(buf.st_atime);
    print(buf.st_mtime);
    print(buf.st_ctime);
    exit(0);
}
