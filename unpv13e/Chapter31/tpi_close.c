#include "tpi_daytime.h"
#include <stdio.h>
#include <unistd.h>

// 向对短发送顺序释放
void tpi_close(int fd)
{
    struct T_ordrel_req ordrel_req;
    struct strbuf       ctlbuf;

    ordrel_req.PRIM_type = T_ORDREL_REQ;

    ctlbuf.len = sizeof(struct T_ordrel_req);
    ctlbuf.buf = (char *) &ordrel_req;
    putmsg(fd, &ctlbuf, NULL, 0);

    close(fd);
}
