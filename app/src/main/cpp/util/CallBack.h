//
// Created by ether on 2019/4/15.
//

#ifndef FFMPEGANDOBOE_CALLBACK_H
#define FFMPEGANDOBOE_CALLBACK_H


#include <jni.h>

class CallBack {
public:

    enum THREAD_TYPE {
        MAIN_THREAD,
        CHILD_THREAD
    };

    CallBack(JavaVM *jvmParam, JNIEnv *envParam, jobject objParam);

    ~CallBack();


    void onDraw(THREAD_TYPE threadType, char* dataPos);
private:
    JavaVM *jvm;
    JNIEnv *env;
    jobject obj;
    jclass jlz;

    jmethodID drawId;
};


#endif //FFMPEGANDOBOE_CALLBACK_H
