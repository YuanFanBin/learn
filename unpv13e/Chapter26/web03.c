#include "../lib/error.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE     4096        /* max text line length */
#define MAXFILES     20
#define SERV        "80"

#define min(a, b)   ((a) < (b) ? (a) : (b))

struct file {
    char      *f_name;  // filename
    char      *f_host;  // hostname or IP address
    int        f_fd;    // descriptor
    int        f_flags; // F_xxx below
    pthread_t  f_tid;   // thread ID
} file[MAXFILES];

#define F_CONNECTING 1  // connect() in progress
#define F_READING    2  // connect() complete; now reading
#define F_DONE       4  // all done
// 26.9
#define F_JOINED     8  // main has pthread_join'ed

// 26.9
int             ndone;  // number of terminated threads
pthread_mutex_t ndone_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  ndone_cond  = PTHREAD_COND_INITIALIZER;

#define GET_CMD      "GET %s HTTP/1.0\r\n\r\n"

int nconn, nfiles, nlefttoconn, nlefttoread;

void *do_get_read(void *vptr);
// ../Chapter16/home_page.c
void home_page(const char *, const char *);
// ../Chapter16/write_get_cmd.c
void write_get_cmd(struct file *);
// ../Chapter11/tcp_connect.c
int tcp_connect(const char *host, const char *serv);

int main(int argc, char **argv)
{
    int          i, maxnconn, e;
    pthread_t    tid;
    struct file *fptr;

    if (argc < 5) {
        err_quit("usage: web <#conns> <IPaddr> <homepage> file1 ...");
    }
    maxnconn = atoi(argv[1]);

    nfiles = min(argc - 4, MAXFILES);
    for (i = 0; i < nfiles; i++) {
        file[i].f_name = argv[i + 4];
        file[i].f_host = argv[2];
        file[i].f_flags = 0;
    }
    printf("nfiles = %d\n", nfiles);

    home_page(argv[2], argv[3]);

    nlefttoread = nlefttoconn = nfiles;
    nconn = 0;
    while (nlefttoread > 0) {
        while (nconn < maxnconn && nlefttoconn > 0) {
            // find a file to read
            for (i = 0; i < nfiles; i++) {
                if (file[i].f_flags == 0) {
                    break;
                }
            }
            if (i == nfiles) {
                err_quit("nlefttoconn = %d but nothing found", nlefttoconn);
            }
            file[i].f_flags = F_CONNECTING;
            if ((e = pthread_create(&tid, NULL, &do_get_read, &file[i])) < 0) {
                errno = e;
                err_sys("pthread_create error");
            }
            file[i].f_tid = tid;
            nconn++;
            nlefttoconn--;
        }
        // 26.6 Solaris thr_join替换成pthread_join
        // TODO: Solaris thread.h
        // 在Salaris环境下可以混用pthreah线程和Solaris线程
        // if ((n = thr_join(0, &tid, (void **) &fptr)) != 0) {
        //     errno = n, err_sys("thr_join error");
        // }

        // 26.9
        // wait for thread to terminate
        pthread_mutex_lock(&ndone_mutex);
        while (ndone == 0) {
            pthread_cond_wait(&ndone_cond, &ndone_mutex);
        }
        for (i = 0; i < nfiles; i++) {
            if (file[i].f_flags & F_DONE) {
                pthread_join(file[i].f_tid, (void **) &fptr);
                if (&file[i] != fptr) {
                    err_quit("file[i] != fptr");
                }
                fptr->f_flags = F_JOINED;   // clears F_DONE
                ndone--;
                nconn--;
                nlefttoread--;
                printf("thread id %d for %s done\n", (int) tid, fptr->f_name);
            }
        }
        pthread_mutex_unlock(&ndone_mutex);
    }
    exit(0);
}

void *do_get_read(void *vptr)
{
    int          fd, n;
    char         line[MAXLINE];
    struct file *fptr;

    fptr = (struct file *) vptr;

    fd = tcp_connect(fptr->f_host, SERV);
    fptr->f_fd = fd;
    printf("do_get_read for %s, fd %d, thread %d\n", fptr->f_name, fd, (int)fptr->f_tid);

    write_get_cmd(fptr);    // write() the GET command

    // read server's reply
    for ( ; ; ) {
        if ((n = read(fd, line, MAXLINE)) == 0) {
            break;  // server closed connection
        }
        printf("read %d bytes from %s\n", n, fptr->f_name);
    }
    printf("end-of-file on %s\n", fptr->f_name);
    close(fd);

    // 26.9
    pthread_mutex_lock(&ndone_mutex);
    fptr->f_flags = F_DONE; // clears F_READING
    ndone++;
    pthread_cond_signal(&ndone_cond);
    pthread_mutex_unlock(&ndone_mutex);

    return(fptr);
}
