#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../lib/error.h"

ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd);

int main(int argc, char **argv)
{
    int fd;

    if (argc != 4) {
        err_quit("openfile <sockfd#> <filename> <mode>");
    }
    if ((fd = open(argv[2], atoi(argv[3]))) < 0) {
        exit((errno > 0 ? errno : 255));
    }
    if (write_fd(atoi(argv[1]), "", 1, fd) < 0) {
        exit((errno > 0 ? errno : 255));
    }
    exit(0);
}

// $ gcc openfile.c write_fd.c ../lib/error.c -o openfile
