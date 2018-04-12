
#ifndef __CAR_CChat_H__
#define __CAR_CChat_H__

#include "_Chat.h"
#include "Elastos.CoreLibrary.Core.h"



class CChat;


CARAPI _CChatCreateObject(
    /* [out] */ CChat **newObj);

class _CChat
{
public:

    static _ELASTOS ECode New(
        /* [out] */ IChat** itfObj)
    {
        return NewByFriend(EIID_IChat, (PInterface*)itfObj);
    }

    static _ELASTOS ECode New(
        /* [out] */ IObject** itfObj)
    {
        return NewByFriend(EIID_IObject, (PInterface*)itfObj);
    }

    static _ELASTOS ECode New(
        /* [out] */ Elastos::Core::ISynchronize** itfObj)
    {
        return NewByFriend(Elastos::Core::EIID_ISynchronize, (PInterface*)itfObj);
    }

    static _ELASTOS ECode New(
        /* [out] */ IWeakReferenceSource** itfObj)
    {
        return NewByFriend(EIID_IWeakReferenceSource, (PInterface*)itfObj);
    }

    static ECode NewByFriend(
        /* [out] */ CChat **newObj);

    static ECode NewByFriend(
        /* [in] */ REIID iid,
        /* [out] */ PInterface* itfObj);

};


#endif // __CAR_CChat_H__
