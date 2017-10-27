
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>



ssize_t sock_send_all(int sfd, void const *buf, size_t len)
{
    int nrb;
    char const *p;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    nrb = len;
    p = (char const *)buf;
    while (nrb > 0) {
        int nb;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
        nb = send(sfd, p, nrb, 0);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
        if (nb == -1)
            return -1;
        p += nb;
        nrb -= nb;
    }
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    return len < 0 ? -1 : len;
}

ssize_t sock_recv_all(int sfd, void *buf, size_t len)
{
    int nrb;
    char *p;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    nrb = len;
    p = (char *)buf;
    while (nrb > 0) {
        int nb;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
        nb = recv(sfd, p, nrb, 0);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
        if (nb == 0)
            return 0;
        if (nb == -1)
            return -1;
        p += nb;
        nrb -= nb;
    }
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    return len < 0 ? -1 : len;
}

int sock_make_pack(int type,
        void const *buf, int len,
        void **rbuf, int *rlen)
{
    int32_t _type = type;
    int32_t lenn;
    void *pb;
    printf("TRACE %s:%d \n", __FILE__, __LINE__);

    if (buf == NULL)
        len = 0;

    lenn = sizeof _type + len;

    *rlen = sizeof lenn + lenn;

    printf("type:%d, lenn:%d, rlen:%d, len:%d\n", type, lenn, *rlen, len);
    lenn = htonl(lenn);

    *rbuf = malloc(*rlen);
    if (*rbuf == NULL)
        return -1;


    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    pb = *rbuf;
    pb = mempcpy(pb, &lenn, sizeof lenn);
    pb = mempcpy(pb, &_type, sizeof _type);
    if (buf != NULL)
        memcpy(pb, buf, len);

    return 0;
}

int sock_send_msg(int sfd, int type, void const *buf, int len)
{
    int ret = 0;
    void *_buf;
    int _len;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (sock_make_pack(type, buf, len, &_buf, &_len))
        return -1;

    int32_t l = *(int32_t*)_buf;
    int32_t t = *(int32_t*)(_buf+4);
    char *s = (char*)_buf + 8;
    printf("send msg %d, %d, %s\n", ntohl(l), t, s);
    if (sock_send_all(sfd, _buf, _len) != _len)
        ret = -1;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    free(_buf);
    printf("sock_send_msg return %d\n", ret);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    return ret;
}

int sock_recv_msg(int sfd, int *type, void **buf, int *len)
{
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    int _type = -1;
    void *__buf = 0;
    int32_t __len = 0;
    ssize_t nb;
    int32_t _len;
    char *_buf = NULL;
    char const *p;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    nb = sock_recv_all(sfd, &_len, sizeof _len);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (nb == 0)
	goto out;
    if (nb != sizeof _len)
	return -1;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    _len = ntohl(_len);
    printf("sock recv msg len:%d\n", _len);
    _buf = (char*)malloc(_len);
    if (_buf == NULL)
        return -1;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    nb = sock_recv_all(sfd, _buf, _len);
    if (nb == 0)
	   goto out;
    if (nb != _len) {
        free (_buf);
        return -1;
    }
    printf("has recved %d\n", nb);

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    __len = _len;

    p = _buf;

    _type = *(int32_t *)p;
    printf("recv type %d\n", _type);
    p += 4;
    __len -= 4;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    __buf = malloc(__len);
    if (__buf == NULL) {
        free (_buf);
        return -1;
    }

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    memcpy(__buf, p, __len);

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
out:
    if (_buf)
        free (_buf);
    *type = _type;
    *buf = __buf;
    *len = __len;
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    
    return 0;
}

int sock_connect(char const *ip, int port)
{
    int sfd;
    struct sockaddr_in saddr;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    sfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (sfd == -1)
        return -1;
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    memset(&saddr, 0, sizeof saddr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    int err = connect(sfd, (struct sockaddr const *)&saddr, sizeof saddr);
    if (err) {
        close(sfd);
    printf("sock_connect 4 error %d\n", errno);
        return -1;
    }
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    return sfd;
}

int sock_close(int sid)
{
    if (sid < 0)
        return 0;
    return close(sid);
}

int sock_get_inet_addr(char *buf, int len)
{
    struct ifaddrs *ap;
    struct ifaddrs *p;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);

    if (getifaddrs(&ap))
        return -1;

    for (p = ap; p != NULL; p = p->ifa_next) {
        struct sockaddr *s = p->ifa_addr;

        if (s->sa_family != AF_INET)
            continue;
        if (strncmp(p->ifa_name, "wlan", 4) != 0)
            continue;
        snprintf(buf, len, "%u.%u.%u.%u",
                (unsigned char)s->sa_data[2],
                (unsigned char)s->sa_data[3],
                (unsigned char)s->sa_data[4],
                (unsigned char)s->sa_data[5]);
        break;
    }
    if (p == NULL)
        return -1;
    return 0;
}

