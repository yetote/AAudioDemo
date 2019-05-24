#include <jni.h>
#include <string>
#include "audio/AudioPlayer.h"
#include "util/CallBack.h"

AudioPlayer *audioPlayer;
CallBack *callBack;
JavaVM *jvm;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    jvm = vm;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_aaudiodemo_AudioPlayer_createEngine(JNIEnv *env, jobject instance, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    callBack = new CallBack(jvm, env, instance);
    audioPlayer = new AudioPlayer(path, callBack);

    env->ReleaseStringUTFChars(path_, path);
}