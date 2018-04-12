#ifndef ___Chat_h__
#define ___Chat_h__

#include <Chat.h>

ELAPI _Impl_AcquireCallbackHandler(PInterface pServerObj, _ELASTOS REIID iid, PInterface *ppHandler);
ELAPI _Impl_CheckClsId(PInterface pServerObj, const _ELASTOS ClassID* pClassid, PInterface *ppServerObj);

CAR_INTERFACE("9A4F1F2C-CCD2-F78C-7FCC-DC80C2048B1C")
IMessageListener : public IInterface
{
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ _ELASTOS REIID riid) = 0;

    static CARAPI_(IMessageListener*) Probe(PInterface pObj)
    {
        if (pObj == NULL) return NULL;
        return (IMessageListener*)pObj->Probe(EIID_IMessageListener);
    }

    static CARAPI_(IMessageListener*) Probe(IObject* pObj)
    {
        if (pObj == NULL) return NULL;
        return (IMessageListener*)pObj->Probe(EIID_IMessageListener);
    }

    static CARAPI_(IMessageListener*) QueryInterface(const Elastos::String& uid)
    {
        return NULL;
    }

    virtual CARAPI MessageRecived(
        /* [in] */ const _ELASTOS String& msg) = 0;

};

CAR_INTERFACE("B6851C35-8312-2415-6EFC-B8AC7966C6FB")
IChat : public IInterface
{
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ _ELASTOS REIID riid) = 0;

    static CARAPI_(IChat*) Probe(PInterface pObj)
    {
        if (pObj == NULL) return NULL;
        return (IChat*)pObj->Probe(EIID_IChat);
    }

    static CARAPI_(IChat*) Probe(IObject* pObj)
    {
        if (pObj == NULL) return NULL;
        return (IChat*)pObj->Probe(EIID_IChat);
    }

    static CARAPI_(IChat*) QueryInterface(const Elastos::String& uid)
    {
        return NULL;
    }

    virtual CARAPI Send(
        /* [in] */ const _ELASTOS String& msg) = 0;

    virtual CARAPI SetMessageListener(
        /* [in] */ IMessageListener * listener) = 0;

};

CAR_INTERFACE("0560522C-0312-2B06-48DC-D063F48725A3")
ICChatClassObject : public IClassObject
{
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ _ELASTOS REIID riid) = 0;

    static CARAPI_(ICChatClassObject*) Probe(PInterface pObj)
    {
        if (pObj == NULL) return NULL;
        return (ICChatClassObject*)pObj->Probe(EIID_ICChatClassObject);
    }

    static CARAPI_(ICChatClassObject*) Probe(IObject* pObj)
    {
        if (pObj == NULL) return NULL;
        return (ICChatClassObject*)pObj->Probe(EIID_ICChatClassObject);
    }

    static CARAPI_(ICChatClassObject*) QueryInterface(const Elastos::String& uid)
    {
        return NULL;
    }

    virtual CARAPI CreateObjectWithDefaultCtor(
        /* [out] */ IInterface ** newObj) = 0;

};

class CChat
{
public:
    static _ELASTOS ECode New(
            /* [out] */ IChat** __object)
    {
        return _CObject_CreateInstance(ECLSID_CChat, RGM_SAME_DOMAIN, EIID_IChat, (PInterface*)__object);
    }

    static _ELASTOS ECode New(
            /* [out] */ IObject** __object)
    {
        return _CObject_CreateInstance(ECLSID_CChat, RGM_SAME_DOMAIN, EIID_IObject, (PInterface*)__object);
    }

    static _ELASTOS ECode New(
            /* [out] */ Elastos::Core::ISynchronize** __object)
    {
        return _CObject_CreateInstance(ECLSID_CChat, RGM_SAME_DOMAIN, Elastos::Core::EIID_ISynchronize, (PInterface*)__object);
    }

    static _ELASTOS ECode New(
            /* [out] */ IWeakReferenceSource** __object)
    {
        return _CObject_CreateInstance(ECLSID_CChat, RGM_SAME_DOMAIN, EIID_IWeakReferenceSource, (PInterface*)__object);
    }

};

#endif // ___Chat_h__
