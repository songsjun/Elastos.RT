#include <jni.h>
#include <string>
#include <_Chat.h>
#include <android/log.h>
#include "elastos.h"
#include "elapi.h"
#include "_ElastosCore.h"
#include "CCarrierListener.h"
#include "CMessageListener.h"
#include "elastos/utility/etl/HashMap.h"

ICarrier* gCarrier = NULL;
IServiceManager* gServiceManager = NULL;

Elastos::Utility::Etl::HashMap<String, IChat*> gInterfaces;

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_elastos_rpcdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C" JNIEXPORT void JNICALL Java_com_elastos_rpcdemo_MainActivity_startCarrier(
        JNIEnv *env,
        jobject jobj,
        jstring path) {

        ECode ec = (ECode) _CCarrier_GetInstance(&gCarrier);
        if (FAILED(ec)) return;

        CCarrierListener* listener = new CCarrierListener(env, jobj);
        ICarrierListener* pLis = ICarrierListener::Probe(listener);
        __android_log_print(ANDROID_LOG_DEBUG, "RPCDemo JNI", "=== probe ICarrierListener:%x", pLis);
        gCarrier->AddCarrierNodeListener(pLis);

        const char* nativeString = env->GetStringUTFChars(path, nullptr);
        gCarrier->Start(String(nativeString));
        env->ReleaseStringUTFChars(path, nativeString);

}

extern "C" JNIEXPORT void JNICALL Java_com_elastos_rpcdemo_MainActivity_stopCarrier(
        JNIEnv *env,
        jobject jobj) {
    if (gCarrier == NULL) return;

    gCarrier->Stop();
    gCarrier->Release();
    gCarrier = NULL;
}

extern "C" JNIEXPORT jint JNICALL Java_com_elastos_rpcdemo_MainActivity_addFriend(
        JNIEnv *env,
        jobject jobj,
        jstring uid) {
    if (gCarrier == NULL) return -1;

    const char* nativeString = env->GetStringUTFChars(uid, nullptr);
    ECode ec = gCarrier->AddFriend(String(nativeString), String("hello"));
    env->ReleaseStringUTFChars(uid, nativeString);
    return ec;
}

extern "C" JNIEXPORT jobject JNICALL Java_com_elastos_rpcdemo_MainActivity_getFriendList(
        JNIEnv *env,
        jobject jobj) {
    if (gCarrier == NULL) return NULL;

    ArrayOf<IFriend*>* friends = NULL;
    ECode ec = gCarrier->GetFriends(&friends);
    if (FAILED(ec) || friends == NULL) return NULL;

    jclass cls_ArrayList = env->FindClass("java/util/ArrayList");
    jmethodID construct = env->GetMethodID(cls_ArrayList, "<init>", "()V");
    jobject obj_ArrayList = env->NewObject(cls_ArrayList, construct,"");
    jmethodID arrayList_add = env->GetMethodID(cls_ArrayList, "add", "(Ljava/lang/Object;)Z");

    jclass cls_friend = env->FindClass("Friend");
    //none argument construct function
    jmethodID construct_user = env->GetMethodID(cls_friend, "<init>", "(Ljava/lang/String;Z)V");

    for (int i = 0; i < friends->GetLength(); i++) {
        IFriend* iFriend = (*friends)[i];

        String uid;
        iFriend->GetUid(&uid);
        Boolean online;
        iFriend->IsOnline(&online);
        jobject obj_friend = env->NewObject(cls_friend, construct_user, env->NewStringUTF(uid.string()), online);
        env->CallObjectMethod(obj_ArrayList, arrayList_add, obj_friend);
    }

    friends->Release();

    return obj_ArrayList;
}


extern "C" JNIEXPORT jint JNICALL Java_com_elastos_rpcdemo_MainActivity_acceptFriend(
        JNIEnv *env,
        jobject jobj,
        jstring uid) {
    if (gCarrier == NULL) return -1;

    const char* nativeString = env->GetStringUTFChars(uid, nullptr);
    ECode ec = gCarrier->AccpetFriendRequest(String(nativeString));
    env->ReleaseStringUTFChars(uid, nativeString);
    return ec;
}

extern "C" JNIEXPORT jstring JNICALL Java_com_elastos_rpcdemo_MainActivity_getAddress(
        JNIEnv *env,
        jobject jobj) {
    if (gCarrier == NULL) return env->NewStringUTF("");

    String address;
    gCarrier->GetAddress(&address);
    return env->NewStringUTF(address.string());
}

extern "C" JNIEXPORT void JNICALL Java_com_elastos_rpcdemo_MainActivity_startServiceManager(
        JNIEnv *env,
        jobject jobj) {
    if (gCarrier == NULL) return;
    if (gServiceManager != NULL) {
        gServiceManager->Release();
    }

}

extern "C" JNIEXPORT void JNICALL Java_com_elastos_rpcdemo_MainActivity_stopServiceManager(
        JNIEnv *env,
        jobject jobj) {
    if (gServiceManager) {
        gServiceManager->Release();
        gServiceManager = NULL;
    }
}

extern "C" JNIEXPORT jint JNICALL Java_com_elastos_rpcdemo_MainActivity_addService(
        JNIEnv *env,
        jobject jobj,
        jstring name) {
    if (gServiceManager == NULL) return -1;

    IChat* chat;
    ECode ec = CChat::New(&chat);
    if (FAILED(ec)) return ec;

    CMessageListener* listener = new CMessageListener(env, jobj);
    chat->SetMessageListener(IMessageListener::Probe(listener));

    const char* nativeString = env->GetStringUTFChars(name, nullptr);
    ec = gServiceManager->AddService(String(nativeString), chat);
    env->ReleaseStringUTFChars(name, nativeString);
    chat->Release();
    return ec;
}

extern "C" JNIEXPORT jint JNICALL Java_com_elastos_rpcdemo_MainActivity_getService(
        JNIEnv *env,
        jobject jobj,
        jstring uid,
        jstring name) {
    if (gServiceManager == NULL) return -1;

    IInterface* pInterface;
    const char* uidstr = env->GetStringUTFChars(uid, nullptr);
    const char* namestr = env->GetStringUTFChars(name, nullptr);
    ECode ec = gServiceManager->GetService(String(uidstr), String(namestr), &pInterface);
    env->ReleaseStringUTFChars(name, namestr);
    if (FAILED(ec)) {
        env->ReleaseStringUTFChars(name, uidstr);
        return ec;
    }

    IChat* pChat = IChat::Probe(pInterface);
    if (pChat == NULL) {
        env->ReleaseStringUTFChars(name, uidstr);
        return -1;
    }
    gInterfaces[String(uidstr)] = pChat;
    return NOERROR;
}

extern "C" JNIEXPORT jint JNICALL Java_com_elastos_rpcdemo_MainActivity_sendMessage(
        JNIEnv *env,
        jobject jobj,
        jstring uid,
        jstring msg) {
    if (gServiceManager == NULL) return -1;

    const char* uidstr = env->GetStringUTFChars(uid, nullptr);
    IChat* pInterface = gInterfaces[String(uidstr)];
    if (pInterface == NULL) {
        env->ReleaseStringUTFChars(uid, uidstr);
        return -1;
    }
    const char* msgstr = env->GetStringUTFChars(msg, nullptr);
    ECode ec = pInterface->Send(String(msgstr));
    env->ReleaseStringUTFChars(uid, uidstr);
    env->ReleaseStringUTFChars(msg, msgstr);
    return ec;
}

