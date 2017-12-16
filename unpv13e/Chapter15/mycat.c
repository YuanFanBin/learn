#include <fcntl.h>
#include <unistd.h>
#include "../lib/error.h"

#define BUFFSIZE    8192        /* buffer size for reads and writes */

int my_open(const char *, int);

int main(int argc, char **argv)
{
    int     fd, n;
    char    buff[BUFFSIZE];

    if (argc != 2) {
        err_quit("usage: mycat <pathname>");
    }
    if ((fd = my_open(argv[1], O_RDONLY)) < 0) {
        err_sys("cannot open %s", argv[1]);
    }
    while ((n = read(fd, buff, BUFFSIZE)) > 0) {
        write(STDOUT_FILENO, buff, n);
    }
}

// $ gcc mycat.c myopen.c read_fd.c ../lib/error.c -o mycat
