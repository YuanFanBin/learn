#include "../lib/error.h"
#include "tpi_daytime.h"
#include <string.h>

void tpi_bind(int fd, const void *addr, size_t addrlen)
{
    struct {
        struct T_bind_req msg_hdr;
        char              addr[128];
    } bind_req;
    struct {
        struct T_bind_ack msg_hdr;
        char              addr[128];
    } bind_ack;
    struct strbuf       ctlbuf;
    struct T_error_ack *error_ack;
    int                 flags;

    bind_req.msg_hdr.PRIM_type = T_BIND_REQ;
    bind_req.msg_hdr.ADDR_length = addrlen;
    bind_req.msg_hdr.ADDR_offset = sizeof(struct T_bind_req);
    bind_req.msg_hdr.CONIND_number = 0;
    memcpy(bind_req.addr, addr, addrlen);   // sockaddr_in{}

    ctlbuf.len = sizeof(struct T_bind_req) + addrlen;
    ctlbuf.buf = (char *) &bind_req;
    putmsg(fd, &ctlbuf, NULL, 0);

    ctlbuf.maxlen = sizeof(bind_ack);
    ctlbuf.len = 0;
    ctlbuf.buf = (char *) &bind_ack;
    flags = RS_HIPRI;
    getmsg(fd, &ctlbuf, NULL, &flags);

    if (ctlbuf.len < (int) sizeof(long)) {
        err_quit("bad length from gtmsg");
    }

    switch (bind_ack.msg_hdr.PRIM_type) {
        case T_BIND_ACK:
            return;

        case T_ERROR_ACK:
            if (ctlbuf.len < (int) sizeof(struct T_error_ack)) {
                err_quit("bad length for T_ERROR_ACK");
            }
            error_ack = (struct T_error_ack *) &bind_ack.msg_hdr;
            err_quit("T_ERROR_ACK from bind (%d, %d)", error_ack->TLI_error, error_ack->UNIX_error);

        default:
            err_quit("unexpected message type: %d", bind_ack.msg_hdr.PRIM_type);
    }
}
