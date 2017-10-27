
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <elaatomics.h>
#include "prxstub.h"
#include "rot.h"

#if defined(__USE_REMOTE_SOCKET)

#ifdef _MSC_VER
# include <winsock2.h>
#else
# include <arpa/inet.h>
#endif

#include <uv.h>

#include "sock.h"

#endif

ECode LookupModuleInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIModuleInfo **ppModuleInfo);

ECode LookupClassInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIClassInfo **ppClassInfo);

ECode AcquireClassInfo(
    /* [in] */ const ClassID & classId,
    /* [out] */ CIClassInfo **ppClassInfo);

void FlatModuleInfo(
    /* [in] */ CIModuleInfo *pSrcModInfo,
    /* [out] */ CIModuleInfo *pDestModInfo);

void *GetUnalignedPtr(void *pPtr);

ECode CInterfaceStub::UnmarshalIn(
    /* [in] */ const CIMethodInfo *pMethodInfo,
    /* [in] */ CRemoteParcel *pParcel,
    /* [in, out] */ MarshalHeader *pOutHeader,
    /* [in, out] */ UInt32 *puArgs)
{
    return Stub_ProcessUnmsh_In(
            pMethodInfo,
            (IParcel*)pParcel,
            (UInt32 *)pOutHeader,
            puArgs);
}

ECode CInterfaceStub::MarshalOut(
    /* [in] */ UInt32 *puArgs,
    /* [in] */ MarshalHeader *pInHeader,
    /* [out] */ MarshalHeader *pOutHeader,
    /* [in] */ Boolean bOnlyReleaseIn,
    /* [in, out] */ CRemoteParcel* pParcel)
{
    MarshalHeader *pHeader;
    ECode ec;

    ec = Stub_ProcessMsh_Out(
            &(m_pInfo->methods[pInHeader->m_hMethodIndex - 4]),
            puArgs,
            (UInt32 *)pOutHeader,
            bOnlyReleaseIn,
            (IParcel*)pParcel);

    if (pParcel && SUCCEEDED(ec)) {
        pHeader = pParcel->GetMarshalHeader();
        assert(pHeader != NULL);
        pHeader->m_uMagic = pInHeader->m_uMagic;
        pHeader->m_hInterfaceIndex = pInHeader->m_hInterfaceIndex ;
        pHeader->m_hMethodIndex = pInHeader->m_hMethodIndex;
    }

    return ec;
}

CObjectStub::CObjectStub() :
    m_pInterfaces(NULL),
    m_bRequestToQuit(FALSE),
    m_cRef(1)
{
    if (sem_init(&m_sem, 0, 0) < 0) {
        perror("sem_init()");
    }
}

CObjectStub::~CObjectStub()
{
    if (m_pInterfaces) {
        delete [] m_pInterfaces;
    }

    UnregisterExportObject(m_connName);

    sem_destroy(&m_sem);
    pthread_exit((void*)0);
}

PInterface CObjectStub::Probe(
    /* [in] */ REIID riid)
{
    Int32 n;

    if (riid == EIID_IInterface) {
        return (IInterface*)(IStub*)this;
    }

    for (n = 0; n < m_cInterfaces; n++) {
        if (riid == m_pInterfaces[n].m_pInfo->iid) {
            break;
        }
    }
    if (n == m_cInterfaces) {
        MARSHAL_DBGOUT(MSHDBG_WARNING, printf(
                "Stub: Probe failed, iid: "));
        MARSHAL_DBGOUT(MSHDBG_WARNING, DUMP_GUID(riid));

        return NULL;
    }

    return m_pInterfaces[n].m_pObject;
}

UInt32 CObjectStub::AddRef(void)
{
    Int32 lRefs = atomic_inc(&m_cRef);

    MARSHAL_DBGOUT(MSHDBG_CREF, printf(
            "Stub AddRef: %d\n", lRefs));
    return (UInt32)lRefs;
}

UInt32 CObjectStub::Release(void)
{
    Int32 lRefs = atomic_dec(&m_cRef);

    if (lRefs == 0) {
        MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
                "Stub destructed.\n"));
        delete this;

        return 0;
    }

    MARSHAL_DBGOUT(MSHDBG_CREF, printf(
            "Stub Release: %d\n", lRefs));
    return (UInt32)lRefs;
}

ECode CObjectStub::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

#ifdef _arm
ECode CObjectStub::Invoke(
    /* [in] */ void *pInData,
    /* [in] */ UInt32 uInSize,
    /* [out] */ CRemoteParcel **ppParcel)
{
    return _Invoke(pInData, uInSize, ppParcel);
}
#endif //_arm

#ifdef _x86
ECode CObjectStub::Invoke(
    /* [in] */ void *pInData,
    /* [in] */ UInt32 uInSize,
    /* [out] */ CRemoteParcel **ppParcel)
#elif defined(_arm)
ECode CObjectStub::_Invoke(
    /* [in] */ void *pInData,
    /* [in] */ UInt32 uInSize,
    /* [out] */ CRemoteParcel **ppParcel)
#elif defined(_mips)
ECode CObjectStub::Invoke(
    /* [in] */ void *pInData,
    /* [in] */ UInt32 uInSize,
    /* [out] */ CRemoteParcel **ppParcel)
#else
#error unknown architecture
#endif
{
    assert(pInData != NULL);

    MarshalHeader *pInHeader = NULL, *pOutHeader = NULL;
    UInt32 *puArgs, uMethodAddr, uMethodIndex;
    ECode ec, orgec = NOERROR;
    CInterfaceStub *pCurInterface;
    const CIMethodInfo *pMethodInfo;
    CRemoteParcel *pParcel = NULL;

#ifdef _x86
    UInt32 uEAX, uEDX, uECX, uESP;
#elif defined(_arm)
    struct ArgumentBuffer
    {
        UInt32    r0;
        UInt32    r1;
        UInt32    r2;
        UInt32    r3;
    } argbuf;
#if defined(_GNUC)
    UInt32 r0, r1, r2, r3, ip, lr, sp;
#else
#error Unknown C++ compiler
#endif // defined(_GNUC)
#elif defined(_mips)
    UInt32 uV0, uV1;
    UInt32 uA0, uA1, uA2, uA3;
    UInt32 uT0, uT1, uT2, uT3, uT4, uT5, uT6, uT7, uT8, uT9;
    UInt32 uSp;
#else
#error unknown architecture
#endif
    bool_t bOnlyReleaseIn = FALSE;

    MARSHAL_DBGOUT(MSHDBG_NORMAL,
            printf("Stub: in Invoke: isize(%d)\n", uInSize));

    pInHeader = (MarshalHeader *)pInData;
    if (pInHeader->m_uMagic != MARSHAL_MAGIC) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Stub: invalid magic - %x\n", pInHeader->m_uMagic));
        goto ErrorExit;
    }

#if defined(_DEBUG)
    if (uInSize < sizeof(MarshalHeader)) {
        goto ErrorExit;
    }
    if (uInSize != pInHeader->m_uInSize) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Stub: in size error - %d:%d\n",
                uInSize, pInHeader->m_uInSize + sizeof(MarshalHeader) - 1));
        goto ErrorExit;
    }
    if (pInHeader->m_hInterfaceIndex >= m_cInterfaces) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Stub: interface index error - %d:%d\n",
                pInHeader->m_hInterfaceIndex, m_cInterfaces));
        goto ErrorExit;
    }
    MARSHAL_DBGOUT(MSHDBG_NORMAL,
            printf("Stub: interface idx(%d), method idx(%d)\n",
            pInHeader->m_hInterfaceIndex, pInHeader->m_hMethodIndex));
#endif

    if (pInHeader->m_hMethodIndex >= 4) {
        pCurInterface = &(m_pInterfaces[pInHeader->m_hInterfaceIndex]);

        uMethodIndex = pInHeader->m_hMethodIndex - 4;
        if (uMethodIndex >= pCurInterface->m_pInfo->methodNumMinus4) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub: method index out of range - %d:%d\n",
                    uMethodIndex,
                    pCurInterface->m_pInfo->methodNumMinus4));
            goto ErrorExit;
        }

        uMethodAddr =
                pCurInterface->m_pInterface->m_vPtr->m_vTable[uMethodIndex + 4];

        if (0 != pInHeader->m_uOutSize) {
            pOutHeader = (MarshalHeader *)alloca(pInHeader->m_uOutSize);
            if (!pOutHeader) {
                MARSHAL_DBGOUT(MSHDBG_ERROR,
                        printf("Stub error: alloca() failed.\n"));
                ec = E_OUT_OF_MEMORY;
                goto ErrorExit;
            }
        }

        pMethodInfo = &((pCurInterface->m_pInfo)->methods[uMethodIndex]);
        uInSize = GET_LENGTH(pMethodInfo->reserved1) * 4 + 4;
        MARSHAL_DBGOUT(MSHDBG_NORMAL,
                printf("Stub: method args stack size (%d)\n", uInSize));
        puArgs = (UInt32 *)alloca(uInSize);
        if (!puArgs) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: alloca() failed.\n"));
            ec = E_OUT_OF_MEMORY;
            goto ErrorExit;
        }
#ifdef _x86
        GET_REG(ESP, puArgs);
#endif

        pParcel = new CRemoteParcel((UInt32*)pInHeader);
        ec = pCurInterface->UnmarshalIn(pMethodInfo,
                                        pParcel,
                                        pOutHeader,
                                        puArgs + 1);
        if (FAILED(ec)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: UnmarshalIn() failed.\n"));
            goto ErrorExit;
        }

        *puArgs = (UInt32)pCurInterface->m_pObject; // fill this

        MARSHAL_DBGOUT(MSHDBG_NORMAL,
                printf("Stub: invoke method - args(%x), addr(%x) \n",
                puArgs, uMethodAddr));

#ifdef _x86
        GET_REG(EAX, uEAX);
        GET_REG(EDX, uEDX);
        GET_REG(ECX, uECX);
        GET_REG(ESP, uESP);
        assert(uESP == (UInt32)puArgs);
        // NOTE: we should promise STUB_INVOKE_METHOD just occures
        // in use space , if it used in kernel mode , the return
        // args in stack have the possibilty for being modified
        //
        STUB_INVOKE_METHOD(ec, puArgs, uMethodAddr);
        SET_REG(ECX, uECX);
        SET_REG(EDX, uEDX);
        SET_REG(EAX, uEAX);
#elif defined(_arm)

#if defined(_GNUC)
        if (sizeof(UInt32) * 4 >= uInSize) {
            GET_REG(r0, r0);
            GET_REG(r1, r1);
            GET_REG(r2, r2);
            GET_REG(r3, r3);
            GET_REG(ip, ip);
            GET_REG(lr, lr);
            STUB_INVOKE_METHOD1(ec, puArgs, uMethodAddr);
            SET_REG(lr, lr);
            SET_REG(ip, ip);
            SET_REG(r3, r3);
            SET_REG(r2, r2);
            SET_REG(r1, r1);
            SET_REG(r0, r0);
        }
        else {
            argbuf = *(ArgumentBuffer *)puArgs;
            GET_REG(r0, r0);
            GET_REG(r1, r1);
            GET_REG(r2, r2);
            GET_REG(r3, r3);
            GET_REG(ip, ip);
            GET_REG(lr, lr);
            GET_REG(sp, sp);
            STUB_INVOKE_METHOD2(ec, puArgs, uMethodAddr);
            SET_REG(sp, sp);
            SET_REG(lr, lr);
            SET_REG(ip, ip);
            SET_REG(r3, r3);
            SET_REG(r2, r2);
            SET_REG(r1, r1);
            SET_REG(r0, r0);
            *(ArgumentBuffer *)puArgs = argbuf;
        }
#else

#error Unknown C++ compiler

#endif // defined(_GNUC)

#elif defined(_mips)
        GET_REG($2, uV0);
        GET_REG($3, uV1);
        GET_REG($4, uA0);
        GET_REG($5, uA1);
        GET_REG($6, uA2);
        GET_REG($7, uA3);
        GET_REG($8, uT0);
        GET_REG($9, uT1);
        GET_REG($10, uT2);
        GET_REG($11, uT3);
        GET_REG($12, uT4);
        GET_REG($13, uT5);
        GET_REG($14, uT6);
        GET_REG($15, uT7);
        GET_REG($24, uT8);
        GET_REG($25, uT9);
        GET_REG($29, uSp);
        STUB_INVOKE_METHOD(ec, puArgs, uMethodAddr);
        SET_REG($29, uSp);
        SET_REG($2, uV0);
        SET_REG($3, uV1);
        SET_REG($4, uA0);
        SET_REG($5, uA1);
        SET_REG($6, uA2);
        SET_REG($7, uA3);
        SET_REG($8, uT0);
        SET_REG($9, uT1);
        SET_REG($10, uT2);
        SET_REG($11, uT3);
        SET_REG($12, uT4);
        SET_REG($13, uT5);
        SET_REG($14, uT6);
        SET_REG($15, uT7);
        SET_REG($24, uT8);
        SET_REG($25, uT9);
#else
#error unknown architecture
#endif

        if (FAILED(ec) && GET_IN_INTERFACE_MARK(pMethodInfo->reserved1)) {
            bOnlyReleaseIn = TRUE;
        }

        // puArgs + 1 , skip this pointer
        if ((pOutHeader && SUCCEEDED(ec))
            || GET_IN_INTERFACE_MARK(pMethodInfo->reserved1)) {

            if (pOutHeader && SUCCEEDED(ec)) {
                *ppParcel = new CRemoteParcel();
                if (*ppParcel == NULL) {
                    ec = E_OUT_OF_MEMORY;
                    goto ErrorExit;
                }
            }

            orgec = pCurInterface->MarshalOut(puArgs + 1,
                                              pInHeader,
                                              pOutHeader,
                                              bOnlyReleaseIn,
                                              *ppParcel);
            if (FAILED(orgec)) {
                MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                        "Stub: marshal out args fail, hr(%x)\n", ec));
                ec = orgec;
                goto ErrorExit;
            }
        }
    }
#if defined(__USE_REMOTE_SOCKET)
    else if (pInHeader->m_hMethodIndex == 0) {
        IInterface *pInterface;
        InterfaceID *pIID = (InterfaceID *)(pInHeader + 1);

        pInterface = m_pInterfaces[0].m_pObject->Probe(*pIID);
        if (pInterface == NULL) {
            ec = E_NO_INTERFACE;
            goto ErrorExit;
        }

        *ppParcel = new CRemoteParcel();
        if (*ppParcel == NULL) {
            ec = E_OUT_OF_MEMORY;
            goto ErrorExit;
        }

        pOutHeader = (*ppParcel)->GetMarshalHeader();
        memcpy(pOutHeader, pInHeader, sizeof(MarshalHeader));
        ec = (*ppParcel)->WriteInterfacePtr(pInterface);
        if (FAILED(ec)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf(
                        "Stub error(%x): Parcel->WriteInterfacePtr().\n",
                        ec));
            // Here do not need to delete ppParcel for caller will do it.
        }
    }
#endif // __USE_REMOTE_SOCKET)
    else {
        if (pInHeader->m_hMethodIndex == 0) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: Remote Probe().\n"));
        }
        else if (pInHeader->m_hMethodIndex == 1) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: Remote AddRef().\n"));
        }
        else if (pInHeader->m_hMethodIndex == 2) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: Remote Release().\n"));
        }
        else if (pInHeader->m_hMethodIndex == 3) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: Remote GetInterfaceID().\n"));
        }
        ec = E_INVALID_OPERATION;    // Do not allow remote Probe().
        goto ErrorExit;
    }

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "Stub: invoke server method ok, _SysReply\n"));

ErrorExit:

    if (pParcel != NULL) delete pParcel;

    return ec;
}

ECode CObjectStub::GetClassID(
    /* [out] */ EMuid *pClsid)
{
    assert(pClsid != NULL);

    *pClsid = ((CIClassInfo*)m_pInfo)->clsid;
    return NOERROR;
}

ECode CObjectStub::GetClassInfo(
    /* [out] */ CIClassInfo **ppClassInfo)
{
    assert(ppClassInfo != NULL);

    *ppClassInfo = m_pInfo;
    return NOERROR;
}

ECode CObjectStub::GetInterfaceIndex(
    /* [in] */ IInterface *pObj,
    /* [out] */ UInt32 *puIndex)
{
    for (UInt32 n = 0; n < (UInt32)m_cInterfaces; n++) {
        if (m_pInterfaces[n].m_pObject == pObj) {
            *puIndex = n;
            return NOERROR;
        }
    }
    MARSHAL_DBGOUT(MSHDBG_WARNING, printf(
        "Stub: InterfaceIndex failed - pObj(%x)\n", pObj));

    return E_NO_INTERFACE;
}

#if !defined(__USE_REMOTE_SOCKET)

DBusHandlerResult CObjectStub::S_HandleMessage(
    /* [in] */ DBusConnection *pconn,
    /* [in] */ DBusMessage* pmsg,
    /* [in] */ void *arg)
{
    DBusMessage *pReply;
    DBusMessageIter args;
    DBusMessageIter subarg;
    void *pOutBuffer;
    Int32 outSize;
    dbus_uint32_t serial = 0;
    MarshalHeader *pheader  = NULL;
    UInt32 uInSize = 0;
    EMuid clsid;
    CIModuleInfo *pSrcModInfo;
    CIModuleInfo *pDestModInfo;
    CObjectStub *pThis = (CObjectStub*)arg;
    CRemoteParcel *pParcel = NULL;
    ECode ec;

    // check this is a method call for the right interface & method
    if (dbus_message_is_method_call(pmsg,
                                    STUB_OBJECT_DBUS_INTERFACE,
                                    "GetClassInfo")) {

        pThis->GetClassID(&clsid);
        if (FAILED(LookupModuleInfo(clsid, &pSrcModInfo))) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Lookup module info fail.\n"));
            return DBUS_HANDLER_RESULT_HANDLED;
        }

        pDestModInfo = (CIModuleInfo *)calloc(pSrcModInfo->totalSize, 1);
        FlatModuleInfo(pSrcModInfo, pDestModInfo);

        // create a reply from the message
        pReply = dbus_message_new_method_return(pmsg);

        // add the arguments to the reply
        dbus_message_iter_init_append(pReply, &args);

        dbus_message_iter_open_container(&args,
                                         DBUS_TYPE_ARRAY,
                                         DBUS_TYPE_BYTE_AS_STRING,
                                         &subarg);

        dbus_message_iter_append_fixed_array(&subarg,
                                             DBUS_TYPE_BYTE,
                                             &pDestModInfo,
                                             pDestModInfo->totalSize);

        dbus_message_iter_close_container(&args, &subarg);

        // send the reply && flush the connection
        if (!dbus_connection_send(pconn, pReply, &serial)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Out Of Memory.\n"));
        }
        dbus_connection_flush(pconn);

        // free the reply
        dbus_message_unref(pReply);

        free(pDestModInfo);
    }
    else if (dbus_message_is_method_call(pmsg,
                                    STUB_OBJECT_DBUS_INTERFACE,
                                    "Invoke")) {
        // read the arguments
        if (!dbus_message_iter_init(pmsg, &args)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Message has no arguments.\n"));
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        if (DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type(&args)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Argument is not array.\n"));
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        dbus_message_iter_recurse(&args, &subarg);
        dbus_message_iter_get_fixed_array(&subarg, (void**)&pheader, (int*)&uInSize);

        MARSHAL_DBGOUT(MSHDBG_NORMAL,
            printf("Call Invoke.\n"));

        ec = pThis->Invoke((void*)pheader, uInSize, &pParcel);

        //reply
        // create a reply from the message
        pReply = dbus_message_new_method_return(pmsg);

        // add the arguments to the reply
        dbus_message_iter_init_append(pReply, &args);

        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &ec)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Out Of Memory.\n"));
            return DBUS_HANDLER_RESULT_HANDLED;
        }

        if (pParcel != NULL) {
            pParcel->GetElementSize(&outSize);
            pParcel->GetElementPayload((Handle32*)&pOutBuffer);

            if (outSize > 0) {
                dbus_message_iter_open_container(&args,
                                                 DBUS_TYPE_ARRAY,
                                                 DBUS_TYPE_BYTE_AS_STRING,
                                                 &subarg);
                dbus_message_iter_append_fixed_array(&subarg,
                                                     DBUS_TYPE_BYTE,
                                                     &pOutBuffer,
                                                     outSize);
                dbus_message_iter_close_container(&args, &subarg);
            }
        }

        // send the reply && flush the connection
        if (!dbus_connection_send(pconn, pReply, &serial)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Out Of Memory.\n"));
        }
        dbus_connection_flush(pconn);

        // free the reply
        dbus_message_unref(pReply);

        if (pParcel != NULL) delete pParcel;
    }
    else if (dbus_message_is_method_call(pmsg,
                                         STUB_OBJECT_DBUS_INTERFACE,
                                         "Release")) {
        MARSHAL_DBGOUT(MSHDBG_NORMAL,
                printf("Stub Release.\n"));

        //bugbug: if there are more than one processes invoke
        //        the stub, then one proxy in a process call "Release"
        //        should not make the stub quit. We should use
        //        reference count to control when to make the stub quit.
        pThis->m_bRequestToQuit = TRUE;
//        if (pThis->Release() == 1) {
//            pThis->m_bRequestToQuit = TRUE;
//        }
    }
    else {
        const char *psig = dbus_message_get_signature(pmsg);
        if (psig != NULL) printf("Signature: %s\n", psig);
//        printf("Destination: %s\n", dbus_message_get_destination(pmsg));
//        printf("Interface: %s\n", dbus_message_get_interface(pmsg));
//        printf("Member: %s\n", dbus_message_get_member(pmsg));
    }

    return DBUS_HANDLER_RESULT_HANDLED;
}

#else

ECode CObjectStub::HandleGetClassInfo(uv_tcp_t *tcp, void const *base, int len)
{
    EMuid clsid;
    CIModuleInfo *pSrcModInfo;
    CIModuleInfo *pDestModInfo;

    this->GetClassID(&clsid);
    if (FAILED(LookupModuleInfo(clsid, &pSrcModInfo))) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf("Lookup module info fail.\n"));
        return NOERROR;
    }

    pDestModInfo = (CIModuleInfo *)calloc(1, pSrcModInfo->totalSize);
    FlatModuleInfo(pSrcModInfo, pDestModInfo);

    if (sock_send_msg(tcp,
                METHOD_GET_CLASS_INFO_REPLY,
                pDestModInfo,
                pDestModInfo->totalSize))
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf("Socket-sending failed.\n"));

    free(pDestModInfo);

    return NOERROR;
}

ECode CObjectStub::HandleInvoke(uv_tcp_t *tcp, void const *base, int len)
{
    void *pOutBuffer;
    Int32 outSize;
    CRemoteParcel *pParcel = NULL;
    ECode ec;

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf("Call Invoke.\n"));

    ec = this->Invoke((void *)base, len, &pParcel);

    int32_t _ec = ec;
    if (pParcel != NULL) {
        pParcel->GetElementSize(&outSize);
        pParcel->GetElementPayload((Handle32*)&pOutBuffer);

        if (outSize > 0) {
            int out_size = sizeof _ec + outSize;
            // char out_buf[out_size];
            char* out_buf = (char*)alloca(out_size * sizeof(char));
            char *p;

            p = out_buf;
            memcpy(p, &_ec, sizeof _ec);
            p += sizeof _ec;
            memcpy(p, pOutBuffer, outSize);


            if (sock_send_msg(tcp, METHOD_INVOKE_REPLY, out_buf, out_size))
                MARSHAL_DBGOUT(MSHDBG_ERROR,
                        printf("Socket-sending failed.\n"));


        }
    } else {
	    if (sock_send_msg(tcp, METHOD_INVOKE_REPLY, &_ec, sizeof _ec))
		    MARSHAL_DBGOUT(MSHDBG_ERROR,
				    printf("Socket-sending failed.\n"));
    }

    if (pParcel != NULL)
        delete pParcel;

    return NOERROR;
}

ECode CObjectStub::HandleRelease(uv_tcp_t *tcp, void const *base, int len)
{
    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf("Stub Release.\n"));

    //bugbug: if there are more than one processes invoke
    //        the stub, then one proxy in a process call "Release"
    //        should not make the stub quit. We should use
    //        reference count to control when to make the stub quit.
    //this->m_bRequestToQuit = TRUE;

    if (this->Release() == 0 && m_loop != NULL)
        uv_stop(m_loop);

    return NOERROR;
}

void CObjectStub::S_TCPReadAllocCB(uv_handle_t *handle,
        size_t suggestedSize,
        uv_buf_t *buf)
{
    buf->base = (char *)malloc(suggestedSize);
    if (buf->base == NULL)
        abort();
    buf->len = suggestedSize;
}

struct _BD {
    uv_buf_t buf;
    void *data;
};

void CObjectStub::S_TCPReadCB(uv_stream_t *stream,
        ssize_t count,
        uv_buf_t const *buf)
{
    ECode ec = NOERROR;
    uv_tcp_t *tcp;
    struct _BD *bd;
    CObjectStub *pThis;
    uint32_t len;
    size_t _len_;
    size_t _len;
    char *_base, *p;

    tcp = (uv_tcp_t *)stream;
    bd = (struct _BD *)tcp->data;
    pThis = (CObjectStub *)bd->data;
    if (count < 0) {
        uv_close((uv_handle_t *)tcp, NULL);
        goto out;
    }
    if (count == 0)
        goto out;

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
        int type;
        size_t __len;
        char *__base;

        p = (char *)_base;
        p += sizeof(int32_t);
        _len -= sizeof(int32_t);
        type = *(int32_t *)p;
        p += sizeof(int32_t);
        _len -= sizeof(int32_t);
        switch (type) {
        case METHOD_GET_CLASS_INFO:
            ec = pThis->HandleGetClassInfo(tcp, p, _len);
            break;
        case METHOD_INVOKE:
            ec = pThis->HandleInvoke(tcp, p, _len);
            break;
        case METHOD_RELEASE:
            ec = pThis->HandleRelease(tcp, p, _len);
            break;
        default:
            ec = NOERROR;
            break;
        }
        if (FAILED(ec))
            abort();

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
    }

out:
    free(buf->base);
    if (count < 0)
        free(bd->buf.base);
}

void CObjectStub::S_ListenCB(uv_stream_t *server, int status)
{
    uv_tcp_t *tcp;
    CObjectStub *pThis;
    uv_tcp_t *_tcp;
    struct _BD *bd;

    tcp = (uv_tcp_t *)server;
    pThis = (CObjectStub *)tcp->data;
    if (status != 0)
        return;
    _tcp = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
    if (_tcp == NULL)
        abort();
    if (uv_tcp_init(tcp->loop, _tcp))
        goto out1;

    bd = (struct _BD *)malloc(sizeof(struct _BD));
    if (bd == NULL)
        goto out2;
    bd->buf.base = NULL;
    bd->buf.len = 0;
    _tcp->data = bd;

    if (uv_accept(server, (uv_stream_t *)_tcp))
        goto out3;
    pThis->AddRef();
    bd->data = pThis;
    if (uv_read_start((uv_stream_t *)_tcp, &S_TCPReadAllocCB, &S_TCPReadCB)) {
        pThis->Release();
out3:
        free(bd);
out2:
        uv_close((uv_handle_t *)_tcp, NULL);
        uv_run(tcp->loop, UV_RUN_NOWAIT);
out1:
        free(_tcp);
    }
}

#endif

void CObjectStub::S_ServiceRoutine(void *arg)
{

#if defined(__USE_REMOTE_SOCKET)

    CObjectStub *pThis = (CObjectStub *)arg;
    uv_loop_t *loop;
    int err;
    uv_tcp_t tcp;
    struct sockaddr_in addr;
    char ip[16];
    int port;
    struct sockaddr addr2;
    int addr2len = sizeof(struct sockaddr_in);
    char connName[32];
    ECode ec;

    loop = (uv_loop_t *)malloc(sizeof(uv_loop_t));
    if (loop == NULL)
        abort();
    if (uv_loop_init(loop))
        goto out1;
    if ((err = uv_tcp_init(loop, &tcp)))
        goto out2;
    if ((err = uv_tcp_nodelay(&tcp, 1)))
        goto out3;
    if ((err = uv_tcp_keepalive(&tcp, 1, 1)))
        goto out3;
    if ((err = uv_tcp_simultaneous_accepts(&tcp, 1)))
        goto out3;
    if ((err = uv_ip4_addr("0.0.0.0", 0, &addr)))
        goto out3;
    if ((err = uv_tcp_bind(&tcp, (struct sockaddr *)&addr, 0)))
        goto out3;
    pThis->AddRef();
    tcp.data = pThis;
    if ((err = uv_listen((uv_stream_t *)&tcp, 128, &S_ListenCB))) {
        pThis->Release();
        goto out3;
    }
    if (sock_get_inet_addr(ip, sizeof(ip)))
        goto out3;
    pThis->m_loop = loop;
    memset(&addr2, -1, sizeof addr2);
    addr2len = sizeof addr2;
    if (uv_tcp_getsockname(&tcp, &addr2, &addr2len))
	goto out3;
    port = ntohs(((struct sockaddr_in *)&addr2)->sin_port);
    pThis->m_port = port;
    sprintf(connName, "%s:%d", ip, port);
    pThis->m_connName = connName;
    sem_post(&pThis->m_sem);
    uv_run(loop, UV_RUN_DEFAULT);
    pThis->m_loop = NULL;
    ec = UnregisterExportObject(pThis->m_connName);
    if (FAILED(ec))
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                    "Release stub: "
                    "unregister export object failed, ec(%x)\n", ec));
out3:
    uv_close((uv_handle_t *)&tcp, NULL);
    uv_run(loop, UV_RUN_NOWAIT);
out2:
    err = uv_loop_close(loop);
out1:
    free(loop);
    pThis->Release();
    if (err)
        abort();

#else

    DBusError err;
    DBusConnection *pconn;
    DBusObjectPathVTable objectPathVTable;
    const char *uname;

    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    MARSHAL_DBGOUT(MSHDBG_NORMAL,
            printf("Listening for method calls.\n"));

    // initialise the error
    dbus_error_init(&err);

    // connect to the bus and check for errors
#ifdef _MSC_VER
	pconn = dbus_bus_get_private(DBUS_BUS_SESSION, &err);
#else
    pconn = dbus_bus_get_private(DBUS_BUS_SYSTEM, &err);
#endif
    if (dbus_error_is_set(&err)) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Connection Error (%s).\n", err.message));
        dbus_error_free(&err);
        pthread_exit((void*)E_FAIL);
    }

    uname = dbus_bus_get_unique_name(pconn);
    if (uname == NULL) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Get connection name fail.\n"));
        pthread_exit((void*)E_FAIL);
    }
    ((CObjectStub*)arg)->m_connName = uname;

    objectPathVTable.unregister_function = NULL;
    objectPathVTable.message_function = S_HandleMessage;

    dbus_connection_register_object_path(pconn,
            STUB_OBJECT_DBUS_OBJECT_PATH, &objectPathVTable, arg);


    sem_post(&((CObjectStub*)arg)->m_sem);
    // loop, testing for new messages
    while (TRUE){
        DBusDispatchStatus status;

        do{
            dbus_connection_read_write_dispatch(pconn, -1);
        }while((status = dbus_connection_get_dispatch_status(pconn)) \
                == DBUS_DISPATCH_DATA_REMAINS && !((CObjectStub*)arg)->m_bRequestToQuit);

        if (status == DBUS_DISPATCH_NEED_MEMORY){
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("More memory is needed.\n"));
            break;
        }

        if (((CObjectStub*)arg)->m_bRequestToQuit) {
            ECode ec = UnregisterExportObject(((CObjectStub*)arg)->m_connName);
            if (FAILED(ec)) {
                MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                        "Release stub: unregister export object failed, ec(%x)\n", ec));
            }
            ((CObjectStub*)arg)->Release();
            break;
        }
    }

    return (void*)NOERROR;

#endif

}

ECode CObjectStub::StartIPCService()
{

#if defined(__USE_REMOTE_SOCKET)

    this->AddRef();

#endif

    if (uv_thread_create(&m_serviceThread, &S_ServiceRoutine, this)) {

#if defined(__USE_REMOTE_SOCKET)

        this->Release();

#endif

        return E_THREAD_ABORTED;
    }
    sem_wait(&m_sem);
    return NOERROR;
}

ECode CObjectStub::S_CreateObject(
    /* [in] */ IInterface *pObject,
    /* [out] */ IStub **ppIStub)
{
    CObjectStub *pStub;
    CInterfaceStub *pInterfaces;
    IInterface *pObj;
    IObject *pObj1;
    ClassID clsid;
    InterfaceID iid;
    ECode ec = NOERROR;
    CIClassInfo ** ppClassInfo = NULL;
    Int32 n;

    if (!pObject || !ppIStub) {
        return E_INVALID_ARGUMENT;
    }

    *ppIStub = NULL;

    pStub = new CObjectStub();
    if (!pStub) {
        return E_OUT_OF_MEMORY;
    }

    pObj1 = (IObject *)pObject->Probe(EIID_IAspect);
    if (!pObj1) {
        pObj1 = (IObject*)pObject->Probe(EIID_IObject);
    }
    if (!pObj1) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create stub: interface do not support EIID_IObject QI.\n"));
        goto ErrorExit;
    }

    ec = pObj1->GetClassID(&clsid);
    if (FAILED(ec)) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create stub: fail to get object's ClassID.\n"));
        goto ErrorExit;
    }

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "QI class info ok. EMuid is:\n"));
    MARSHAL_DBGOUT(MSHDBG_NORMAL, DUMP_CLSID(clsid));

    ec = pObject->GetInterfaceID(pObject, &iid);
    if (FAILED(ec)) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create stub: interface does not have InterfaceID.\n"));
        goto ErrorExit;
    }
    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "QI EIID info ok. EIID is:\n"));
    MARSHAL_DBGOUT(MSHDBG_NORMAL, DUMP_GUID(iid));

    ec = LookupClassInfo(clsid.clsid, &(pStub->m_pInfo));
    if (FAILED(ec)) {
        ec = AcquireClassInfo(clsid, &(pStub->m_pInfo));
        if (FAILED(ec)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Create Stub: get class info fail, the ec = 0x%08x\n", ec));
            goto ErrorExit;
        }
    }

    ppClassInfo = &(pStub->m_pInfo);

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "Create stub: Get class info ok.\n"));

    pInterfaces = new CInterfaceStub[(*ppClassInfo)->interfaceNum];
    if (!pInterfaces) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf("Create stub: out of memory.\n"));
        ec = E_OUT_OF_MEMORY;
        goto ErrorExit;
    }
    pStub->m_cInterfaces = (*ppClassInfo)->interfaceNum;
    pStub->m_pInterfaces = pInterfaces;

    pObject->AddRef();
    for (n = 0; n < (*ppClassInfo)->interfaceNum; n++) {
        CIInterfaceInfo *pInterfaceInfo =
                (CIInterfaceInfo *)GetUnalignedPtr(
                        (*ppClassInfo)->interfaces + n);
        pObj = pObject->Probe(pInterfaceInfo->iid);
        if (!pObj) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Create stub: no such interface.\n"));
            ec = E_NO_INTERFACE;
            pObject->Release();
            goto ErrorExit;
        }
        pInterfaces[n].m_pInfo = pInterfaceInfo;
        pInterfaces[n].m_pObject = pObj;
    }

    //Start stub ipc service
    ec = pStub->StartIPCService();

    /* Please be careful of the 'm_connName' in the two threads. */
    usleep(300);

    if (FAILED(ec)) {
        pObject->Release();
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf("Stub: start ipc server fail.\n"));
        goto ErrorExit;
    }

    ec = RegisterExportObject(pStub->m_connName, pObject, pStub);
    if (FAILED(ec)) {
        pObject->Release();
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create stub: register export object failed, ec(%x)\n", ec));
        goto ErrorExit;
    }

    *ppIStub = (IStub *)pStub;

    return NOERROR;

ErrorExit:
    delete pStub;
    return ec;
}

