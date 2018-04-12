//
// Created by 林楠 on 2018/4/11.
//

#ifndef RPCDEMO_CMESSAGELISTENER_H
#define RPCDEMO_CMESSAGELISTENER_H

#include <elastos/core/Object.h>
#include <jni.h>
#include "_ElastosCore.h"
#include "elatypes.h"
#include <_Chat.h>

class CMessageListener
    : public Object
    , public IMessageListener
{
public:
    CAR_INTERFACE_DECL()

    CMessageListener(
            JNIEnv *env,
            jobject jobj)
    {
        env->GetJavaVM(&mVm);
        mObj = env->NewGlobalRef(jobj);
    }

    virtual ~CMessageListener()
    {
        GetEnv()->DeleteGlobalRef(mObj);
    }

    CARAPI MessageRecived(
            /* [in] */ const _ELASTOS String& msg);


private:
    JNIEnv* GetEnv();

    void Detach();

private:
    JavaVM* mVm;
    jobject mObj;
};
#endif //RPCDEMO_CMESSAGELISTENER_H
