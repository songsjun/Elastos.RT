

#include "CCarrierListener.h"
#include <Elastos.CoreLibrary.h>
#include "android/log.h"


CAR_INTERFACE_IMPL(CCarrierListener, Object, ICarrierListener)

_ELASTOS ECode CCarrierListener::OnConnectionChanged(
    /* [in] */ _ELASTOS Boolean online)
{
    __android_log_print(ANDROID_LOG_DEBUG, "CCarrierListener", "==== OnConnectionChanged: %d", online);
    JNIEnv* env = GetEnv();

    jclass clazz = env->GetObjectClass(mObj);
    jmethodID method = env->GetMethodID(clazz, "OnConnectionChanged", "(Z)V");
    env->CallVoidMethod(mObj, method, online);
    Detach();

    __android_log_print(ANDROID_LOG_DEBUG, "CCarrierListener", "==== OnConnectionChanged end");
    return NOERROR;
}

_ELASTOS ECode CCarrierListener::OnReady()
{
    return NOERROR;
}

_ELASTOS ECode CCarrierListener::OnFriendRequest(
    /* [in] */ const _ELASTOS String& uid,
    /* [in] */ const _ELASTOS String& hello)
{
    __android_log_print(ANDROID_LOG_DEBUG, "CCarrierListener", "==== OnFriendRequest: %s", uid.string());
    JNIEnv* env = GetEnv();
    jclass clazz = env->GetObjectClass(mObj);
    jmethodID method = env->GetMethodID(clazz, "OnFriendRequest", "(Ljava/lang/String;Ljava/lang/String)V");
    env->CallVoidMethod(mObj, method, env->NewStringUTF(uid.string()), env->NewStringUTF(hello.string()));
    Detach();
    __android_log_print(ANDROID_LOG_DEBUG, "CCarrierListener", "==== OnFriendRequest end");
    return NOERROR;
}

_ELASTOS ECode CCarrierListener::OnFriendConnetionChanged(
    /* [in] */ const _ELASTOS String& uid,
    /* [in] */ _ELASTOS Boolean online)
{
    __android_log_print(ANDROID_LOG_DEBUG, "CCarrierListener", "==== OnFriendConnetionChanged: %s", uid.string());
    JNIEnv* env = GetEnv();
    jclass clazz = env->GetObjectClass(mObj);
    jmethodID method = env->GetMethodID(clazz, "OnFriendConnectionChanged", "(Ljava/lang/String;Z)V");
    env->CallVoidMethod(mObj, method, env->NewStringUTF(uid.string()), online);
    Detach();
    __android_log_print(ANDROID_LOG_DEBUG, "CCarrierListener", "==== OnFriendConnetionChanged end");
    return NOERROR;
}

JNIEnv* CCarrierListener::GetEnv()
{
    JNIEnv* env;
    assert(mVm != NULL);
    mVm->AttachCurrentThread(&env, NULL);
    return env;
}

void CCarrierListener::Detach()
{
    assert(mVm != NULL);
    mVm->DetachCurrentThread();
}

