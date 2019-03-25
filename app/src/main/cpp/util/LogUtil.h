//
// Created by ether on 2019/3/25.
//

#ifndef AAUDIODEMO_LOGUTIL_H
#define AAUDIODEMO_LOGUTIL_H

#endif //AAUDIODEMO_LOGUTIL_H

#include <android/log.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)