//
// Created by ether on 2019/4/15.
//

#include "CallBack.h"



CallBack::CallBack(JavaVM *jvmParam, JNIEnv *envParam, jobject objParam) {
    jvm = jvmParam;
    env = envParam;
    this->obj = env->NewGlobalRef(objParam);
    jlz = env->GetObjectClass(obj);
    drawId = env->GetMethodID(jlz, "onDraw", "([B)V");
}

CallBack::~CallBack() {

}

void CallBack::onDraw(CallBack::THREAD_TYPE threadType, char *dataPos) {
    jbyte *by = (jbyte*)dataPos;
    if (threadType == MAIN_THREAD) {
        jbyteArray jbyteArray1=env->NewByteArray(20);
        env->SetByteArrayRegion(jbyteArray1, 0, 20, by);
        env->CallVoidMethod(obj, drawId, jbyteArray1);
//        env->ReleaseByteArrayElements(jbyteArray1, by, 0);
    } else {
        JNIEnv *env;
        jvm->AttachCurrentThread(&env, 0);
        jbyteArray jbyteArray1=env->NewByteArray(20);
        env->SetByteArrayRegion(jbyteArray1, 0, 20, by);
        env->CallVoidMethod(obj, drawId, jbyteArray1);
//        env->ReleaseByteArrayElements(jbyteArray1, by, 0);
        jvm->DetachCurrentThread();
    }
}
