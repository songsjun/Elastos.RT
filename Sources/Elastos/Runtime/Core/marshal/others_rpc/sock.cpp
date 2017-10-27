
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>



static int sock_make_pack(int type,
        void const *buf, int len,
        void **rbuf, int *rlen)
{
    int32_t _type = type;
    void *_buf;
    int32_t _len;
    char *pb;

    if (buf == NULL)
        len = 0;

    _len = sizeof _len + sizeof _type + len;

    _buf = malloc(_len);
    if (_buf == NULL)
        return -1;

    pb = (char *)_buf;

    memcpy(pb, &_len, sizeof _len);
    pb += sizeof _len;

    memcpy(pb, &_type, sizeof _type);
    pb += sizeof _type;

    if (buf != NULL)
        memcpy(pb, buf, len);

    *rbuf = _buf;
    *rlen = _len;

    return 0;
}

struct _EF {
    int err;
    bool finished;
};

static void write_cb(uv_write_t *req, int status)
{
    struct _EF *ef = (struct _EF *)req->data;

    ef->err = status;
    ef->finished = true;
}

int sock_send_msg(uv_tcp_t *tcp, int type, void const *base, int len)
{
    uv_buf_t buf;
    struct _EF ef;
    uv_write_t req;
    int err;

    if (sock_make_pack(type, base, len, (void **)&buf.base, (int *)&buf.len))
        return -1;
    ef.finished = false;
    req.data = &ef;
    if ((err = uv_write(&req, (uv_stream_t *)tcp, &buf, 1, &write_cb)))
        goto out;
    do
        uv_run(tcp->loop, UV_RUN_ONCE);
    while (!ef.finished);
    err = ef.err;
out:
    free(buf.base);
    return err;
}

static void _tcp_read_alloc_cb(uv_handle_t *handle,
        size_t suggested_size,
        uv_buf_t *buf)
{
    buf->base = (char *)malloc(suggested_size);
    if (buf->base == NULL)
        abort();
    buf->len = suggested_size;
}

struct _BD {
    uv_buf_t buf;
    void *data;
};

struct _BF {
    uv_buf_t buf;
    bool finished;
};

static void _tcp_read_cb(uv_stream_t *stream,
        ssize_t count,
        uv_buf_t const *buf)
{
    uv_tcp_t *tcp;
    struct _BD *bd;
    struct _BF *bf;
    size_t len;
    size_t _len_;
    size_t _len;
    char *_base, *p;

    tcp = (uv_tcp_t *)stream;
    bd = (struct _BD *)tcp->data;
    bf = (struct _BF *)bd->data;
    if (count < 0) {
        uv_close((uv_handle_t *)tcp, NULL);
        free(buf->base);
        return;
    }
    if (count == 0) {
        free(buf->base);
        return;
    }

    if (bd->buf.base == NULL)
        len = *(uint32_t *)buf->base;
    else
        len = *(uint32_t *)bd->buf.base;

    _len_ = bd->buf.len + count;
    _len = _len_ < len ? _len_ : len;

    _base = (char *)malloc(_len);
    if (_base == NULL)
        abort();

    p = _base;
    if (bd->buf.base != NULL) {
        memcpy(p, bd->buf.base, bd->buf.len);
        p += bd->buf.len;
        free(bd->buf.base);
    }
    memcpy(p, buf->base, _len - bd->buf.len);

    if (_len < len) {
        bd->buf.base = _base;
        bd->buf.len = _len;
    } else {
        size_t __len;
        char *__base;

        bf->buf.base = _base;
        bf->buf.len = _len;

        __len = _len_ - len;

        if (__len == 0) {
            bd->buf.base = NULL;
            bd->buf.len = 0;
            goto out;
        }

        __base = (char *)malloc(__len);
        if (__base == NULL)
            abort();

        memcpy(__base, buf->base + len - bd->buf.len, __len);

        bd->buf.base = __base;
        bd->buf.len = __len;

out:
        bf->finished = true;
    }
}

int sock_recv_msg(uv_tcp_t *tcp, int *type, void **base, int *len)
{
    struct _BF bf;
    struct _BD *bd;
    int err;
    char *p;
    int _type;
    void *_base;
    int32_t _len;
    int _err;

    bf.finished = false;
    bd = (struct _BD *)tcp->data;
    bd->data = &bf;
    if ((err = uv_read_start((uv_stream_t *)tcp,
                    &_tcp_read_alloc_cb,
                    &_tcp_read_cb)))
        return err;
    do
        uv_run(tcp->loop, UV_RUN_ONCE);
    while (!bf.finished);
    p = bf.buf.base;
    _len = bf.buf.len;
    p += sizeof(int32_t);
    _len -= sizeof(int32_t);
    _type = *(int32_t *)p;
    p += 4;
    _len -= 4;
    _base = malloc(_len);
    if (_base == NULL) {
        err = -1;
        goto out;
    }
    memcpy(_base, p, _len);
    *type = _type;
    *base = _base;
    *len = _len;
out:
    free(bf.buf.base);
    _err = uv_read_stop((uv_stream_t *)tcp);
    return err ? err : _err;
}

static void _tcp_connect_cb(uv_connect_t *req, int status)
{
    struct _EF *ef = (struct _EF *)req->data;

    ef->err = status;
    ef->finished = true;
}

int sock_connect(uv_tcp_t **tcp, char const *ip, int port)
{
    uv_loop_t *loop;
    int err;
    uv_tcp_t *_tcp;
    struct _BD *bd;
    struct sockaddr_in addr;
    uv_connect_t req;
    struct _EF ef;
    int _err;

    loop = (uv_loop_t *)malloc(sizeof(uv_loop_t));
    if (loop == NULL)
        return -1;
    if ((err = uv_loop_init(loop)))
        goto out1;
    _tcp = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
    if (_tcp == NULL) {
        err = -1;
        goto out2;
    }
    if ((err = uv_tcp_init(loop, _tcp)))
        goto out3;
    bd = (struct _BD *)malloc(sizeof(struct _BD));
    if (bd == NULL)
        goto out4;
    bd->buf.base = NULL;
    bd->buf.len = 0;
    bd->data = NULL;
    _tcp->data = bd;
    if ((err = uv_ip4_addr(ip, port, &addr)))
        goto out5;
    req.data = &ef;
    if ((err = uv_tcp_connect(&req,
                    _tcp,
                    (struct sockaddr *)&addr,
                    &_tcp_connect_cb)))
        goto out4;
    do
        uv_run(loop, UV_RUN_ONCE);
    while (!ef.finished);
    if ((err = ef.err))
        goto out4;
    *tcp = _tcp;
    return 0;
out5:
    free(bd);
out4:
    uv_close((uv_handle_t *)_tcp, NULL);
    uv_run(loop, UV_RUN_NOWAIT);
out3:
    free(_tcp);
out2:
    _err = uv_loop_close(loop);
out1:
    free(loop);
    return err ? err : _err;
}

int sock_close(uv_tcp_t *tcp)
{
    struct _BD *bd;
    int err;
    uv_loop_t *loop;

    bd = (struct _BD *)tcp->data;
    free(bd->buf.base);
    free(bd);
    tcp->data = NULL;
    uv_close((uv_handle_t *)tcp, NULL);
    uv_run(tcp->loop, UV_RUN_NOWAIT);
    if ((err = uv_loop_close(tcp->loop)))
        return err;
    loop = tcp->loop;
    free(tcp);
    free(loop);
    return 0;
}

/*
int sock_get_inet_addr(char *buf, int len)
{
    int err = 0;
    uv_interface_address_t *iaddrs;
    int cnt;
    uv_interface_address_t *p;

    if (uv_interface_addresses(&iaddrs, &cnt))
        return -1;
    for (p = iaddrs; p != NULL; ++p) {
        if (p->address.address4.sin_family != AF_INET)
            continue;
        if (strncmp(p->name, "wlan", 4) != 0)
            continue;
        if (uv_ip4_name(&p->address.address4, buf, len)) {
            err = -1;
            goto out;
        }
        break;
    }
    if (i == cnt)
        err = -1;
out:
    uv_free_interface_addresses(iaddrs, cnt);
    return err;
}
*/

int sock_get_inet_addr(char *buf, int len)
{
    int cnt;
    uv_interface_address_t *iaddrs;
    uv_interface_address_t *p;

    if (uv_interface_addresses(&iaddrs, &cnt)) {
        return -1;
    }
    p = iaddrs;

    for (int i = 0; i < cnt; i++) {
        if (p->address.address4.sin_family == AF_INET) {
            if (strncmp(p->name, "lo", 2) != 0) {
                i = uv_ip4_name(&p->address.address4, buf, len);
                uv_free_interface_addresses(iaddrs, cnt);
                return i;
            }
        }
        p++;
    }

    uv_free_interface_addresses(iaddrs, cnt);
    return -1;
}
