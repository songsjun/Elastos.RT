
#include "CSManS.h"

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../sock/sock.h"



ECode CSManS::AddService(
    /* [in] */ const String& name,
    /* [in] */ InterfacePack * ip)
{
    // TODO: release key.
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    mServices[name] = ip;
    return NOERROR;
}

ECode CSManS::GetService(
    /* [in] */ const String& name,
    /* [out] */ InterfacePack * pIp)
{
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    InterfacePack * ip = mServices[name];
    if (!ip) return E_FAIL;
    memset(pIp, 0, sizeof(InterfacePack));
    pIp->m_clsid = ip->m_clsid;
    pIp->m_uIndex = ip->m_uIndex;

    strcpy(pIp->m_stubConnName, ip->m_stubConnName);         

    return NOERROR;
}

ECode CSManS::RemoveService(
    /* [in] */ const String& name)
{
    void * buf = mServices[name];
    if (buf) {
        mServices.Erase(name);
        free(buf);
    }
    return NOERROR;
}

int CSManS::HandleAddService(int sfd, void const *buf, int len)
{
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    char const *p;
    char const *name;
    Int32 size;
    InterfacePack *ifcp;

    p = (char const *)buf;
    name = p;
    p += strlen(p);
    ++p;
    size = *(int32_t *)p;
    p += 4;

    ifcp = (InterfacePack *)malloc(sizeof(InterfacePack));
    if (ifcp == NULL)
        return -1;

    memset(ifcp, 0, sizeof(InterfacePack));

    memcpy(ifcp, p, size);

    int32_t i32 = AddService(String(name), ifcp);

    if (sock_send_msg(sfd, ADD_SERVICE_REPLY, &i32, sizeof i32))
        return -1;

    return 0;
}

int CSManS::HandleGetService(int sfd, void const *buf, int len)
{
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    char const *name;
    ECode ec;
    InterfacePack ifcp;

    name = (char const *)buf;
 
    ec = GetService(String(name), &ifcp);
    printf("CSManS::HandleGetService name %s, len %d, ec %x\n", name, len, ec);
    int size = sizeof(InterfacePack);

    int _len = 4 + 4 + size;

    char _buf[_len];

    void *p;

    p = _buf;

    int32_t i32;

    i32 = ec;
    p = mempcpy(p, &i32, 4);

    if (SUCCEEDED(ec)) {
        i32 = size;
        p = mempcpy(p, &i32, 4);

        memcpy(p, &ifcp, size);
    } else
        _len = 4;

    if (sock_send_msg(sfd, GET_SERVICE_REPLY, _buf, _len))
        return -1;

    return 0;
}

int CSManS::HandleRemoveService(int sfd, void const *buf, int len)
{
    char const *name;
    ECode ec;

    name = (char const *)buf;
    ec = RemoveService(String(name));

    int32_t i32;

    i32 = ec;

    if (sock_send_msg(sfd, REMOVE_SERVICE_REPLY, &i32, sizeof i32))
        return -1;

    return 0;
}

int CSManS::HandleReceiveEvent(int sfd, int events, void *user_data)
{
    int ret = 0;
    int type;
    void *buf;
    int len;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (sock_recv_msg(sfd, &type, &buf, &len))
        return -1;

    if (type == -1 && buf == NULL && len == 0) {
	if (epoll_ctl(m_epfd, EPOLL_CTL_DEL, sfd, NULL))
	    return -1;
	return 0;
    }

    switch (type) {
    case ADD_SERVICE:
        ret = HandleAddService(sfd, buf, len);
        break;
    case GET_SERVICE:
        ret = HandleGetService(sfd, buf, len);
        break;
    case REMOVE_SERVICE:
        ret = HandleRemoveService(sfd, buf, len);
        break;
    default:
        break;
    }

    free(buf);

    return ret;
}

int CSManS::HandleConnectionEvent(int sfd, int events, void *user_data)
{
    int asfd;
    struct sockaddr_storage saddr;
    socklen_t saddr_len;
    struct epoll_event ree;
    struct epoll_event_data *reed;

    saddr_len = sizeof(struct sockaddr_storage);
    asfd = accept(sfd, (struct sockaddr *)&saddr, &saddr_len);
    if (asfd == -1)
        return -1;

    ree.events = EPOLLIN;

    reed = (struct epoll_event_data *)malloc(sizeof(struct epoll_event_data));
    if (reed == NULL)
        goto out1;

    reed->fd = asfd;
    reed->handle = &CSManS::HandleReceiveEvent;
    reed->user_data = NULL;

    ree.data.ptr = reed;

    if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, asfd, &ree))
        goto out2;

    return 0;

out2:
    free(reed);

out1:
    close(asfd);
    return -1;
}

void *CSManS::S_ServiceRoutine(void *arg)
{
    ECode ec = NOERROR;

    CSManS *pThis;

    struct addrinfo hints, *res;
    int sfd;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    pThis = (CSManS *)arg;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char port[8];
    sprintf(port, "%d", pThis->m_port);
    if (getaddrinfo(NULL, port, &hints, &res)) {
        ec = E_FAIL;
        goto out1;
    }
    printf("TRACE %s:%d \n", __FILE__, __LINE__);

    sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sfd == -1) {
        ec = E_FAIL;
        goto out2;
    }

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    int err = bind(sfd, res->ai_addr, res->ai_addrlen);
    printf("bind error %d \n", err);
    if (err) {
        ec = E_FAIL;
        goto out3;
    }

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (listen(sfd, SOMAXCONN)) {
        ec = E_FAIL;
        goto out3;
    }

    int epfd;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    epfd = epoll_create(1);
    if (epfd == -1) {
        ec = E_FAIL;
        goto out3;
    }

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    pThis->m_epfd = epfd;

    struct epoll_event aee;
    struct epoll_event_data aeed;

    aee.events = EPOLLIN;
    aeed.fd = sfd;
    aeed.handle = &CSManS::HandleConnectionEvent;
    aeed.user_data = NULL;
    aee.data.ptr = &aeed;
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &aee)) {
        ec = E_FAIL;
        goto out4;
    }

    while (true) {
        int nfd;
        struct epoll_event ees[32];
        struct epoll_event *p;
        struct epoll_event *end;

        nfd = epoll_wait(epfd, ees, 32, -1);
        if (nfd == -1) {
            ec = E_FAIL;
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
            break;
        }
        for (p = ees, end = ees + nfd; p < end; ++p) {
            struct epoll_event_data *eed;

            eed = (struct epoll_event_data *)p->data.ptr;
            if ((pThis->*eed->handle)(eed->fd, p->events, eed->user_data)) {
                ec = E_FAIL;
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
                break;
            }
        }
    }

out4:
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    close(epfd);

out3:
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    close(sfd);

out2:
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    freeaddrinfo(res);

out1:
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    //pThis->Release();

    return (void *)ec;
}

CSManS::CSManS()
    : mServices(MAX_SERVICE_NUMBER)
{
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
}

CSManS::~CSManS()
{
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    HashMap<String, InterfacePack*>::Iterator it = mServices.Begin();
    while (it != mServices.End()) {
        if (it->mSecond != NULL) {

            free(it->mSecond);
        }
        it++;
    }
}

ECode CSManS::constructor(
    /* [in] */ Int32 port)
{
    m_port = port;

    //AddRef();

    if (pthread_create(&m_threadID,
                       NULL,
                       S_ServiceRoutine,
                       (void *)this))
    {
        //this->Release();
        return E_FAIL;
    }

    return NOERROR;
}

ECode CSManS::test(void)
{
    return NOERROR;
}

