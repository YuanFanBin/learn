#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

static int
calculate_file(const char *fpath, const struct stat *sb,
        int tflag, struct FTW *ftwbuf)
{
    switch (tflag) {
    case FTW_F:
        switch (sb->st_mode & S_IFMT) {
        case S_IFREG: nreg++; break;
        case S_IFBLK: nblk++; break;
        case S_IFCHR: nchr++; break;
        case S_IFLNK: nslink++; break;
        case S_IFSOCK: nsock++; break;
        case S_IFDIR:   /* directories should have type = FTW_D */
            printf("for S_IFDIR for %s", fpath);
        }
        break;
    case FTW_D:
        ndir++;
        break;
    case FTW_DNR:
        printf("can't read directory %s", fpath);
        break;
    case FTW_NS:
        printf("stat error for %s", fpath);
    default:
        printf("unknown type %d for pathname %s", tflag, fpath);
    }
    return(0);
}

int
main(int argc, char *argv[])
{
    int flags = 0;

    if (nftw((argc < 2) ? "." : argv[1], calculate_file, 20, flags)
            == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);

    }
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0) ntot = 1;
    printf("regular files   = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories     = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special   = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special    = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs           = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("suymbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets         = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);
    exit(EXIT_SUCCESS);

}
