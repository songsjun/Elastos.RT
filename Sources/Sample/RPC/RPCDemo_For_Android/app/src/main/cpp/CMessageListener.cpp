
#include "CMessageListener.h"


CAR_INTERFACE_IMPL(CMessageListener, Object, IMessageListener)

_ELASTOS ECode CMessageListener::MessageRecived(
        /* [in] */ const _ELASTOS String& msg)
{
    JNIEnv* env = GetEnv();
    jclass clazz = env->GetObjectClass(mObj);
    jmethodID method = env->GetMethodID(clazz, "recieveMessage", "(Ljava/lang/String)V");
    env->CallIntMethod(mObj, method, env->NewStringUTF(msg.string()));
    Detach();
    return NOERROR;
}

JNIEnv* CMessageListener::GetEnv()
{
    JNIEnv* env;
    assert(mVm != NULL);
    mVm->AttachCurrentThread(&env, NULL);
    return env;
}

void CMessageListener::Detach()
{
    assert(mVm != NULL);
    mVm->DetachCurrentThread();
}
