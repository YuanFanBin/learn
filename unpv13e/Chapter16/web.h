#ifndef __web_h
#define __web_h

#include "../lib/error.h"
#include <sys/select.h>

#define MAXLINE     4096        /* max text line length */
#define MAXFILES    20
#define SERV        "80"

struct file {
    char *f_name;   // filename
    char *f_host;   // hostname or IPv4/IPv6 address
    int   f_fd;     // descriptor
    int   f_flags;  // F_xxx below
} file[MAXFILES];

#define F_CONNECTING 1 // connect in progress
#define F_READING    2 // connect complete; now reading
#define F_DONE       4 // all done

#define GET_CMD      "GET %s HTTP/1.0\r\nr\rn"

int nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
fd_set rset, wset;

void home_page(const char *, const char *);
void start_connect(struct file *);
void write_get_cmd(struct file *);

#endif // __web_h
