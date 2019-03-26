#include <jni.h>
#include <string>
#include "audio/AudioPlayer.h"

AudioPlayer *audioPlayer;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_aaudiodemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_aaudiodemo_AudioPlayer_createEngine(JNIEnv *env, jobject instance, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    audioPlayer = new AudioPlayer(path);

    env->ReleaseStringUTFChars(path_, path);
}