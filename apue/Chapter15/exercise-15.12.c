#include "apue.h"
#include <sys/ipc.h>
#include <sys/msg.h>

struct mymesg {
    long mtype;
    char mtext[512];
};


int
main(int argc, char *argv[])
{
    int i;
    int msgid;
    struct mymesg mm;

    for (i = 0; i < 5; ++i) {
        if ((msgid = msgget(i, IPC_CREAT | IPC_EXCL)) == -1)
            err_sys("msgget error");
        printf("msgid[%d] = %d\n", i + 1, msgid);
        msgctl(msgid, IPC_RMID, NULL);
    }

    for (i = 0; i < 5; ++i) {
        if ((msgid = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL)) == -1)
            err_sys("msgget error");
        mm.mtype = i + 1;
        sprintf(mm.mtext, "%d", i + 1);
        msgsnd(msgid, &mm, 512, IPC_NOWAIT);
    }
}

/*
 * 参考资料
 * http://blog.csdn.net/guoping16/article/details/6584024
 */

/* 没看到任何现象，估计又写错了。。。 */
