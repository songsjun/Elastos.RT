#ifndef __SOCK_H
# define __SOCK_H

# include <uv.h>



extern int sock_send_msg(uv_tcp_t *tcp,
        int type,
        void const *base,
        int len);

extern int sock_recv_msg(uv_tcp_t *tcp,
        int *type,
        void **base,
        int *len);

extern int sock_connect(uv_tcp_t **tcp, char const *ip, int port);

extern int sock_close(uv_tcp_t *tcp);

extern int sock_get_inet_addr(char *buf, int len);

#endif
