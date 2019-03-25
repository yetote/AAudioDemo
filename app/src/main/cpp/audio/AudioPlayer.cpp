//
// Created by ether on 2019/3/25.
//


#include "AudioPlayer.h"
static const char * audioFormatStr[] = {
        "AAUDIO_FORMAT_INVALID   非法格式", // = -1,
        "AAUDIO_FORMAT_UNSPECIFIED  自动格式", // = 0,
        "AAUDIO_FORMAT_PCM_I16",
        "AAUDIO_FORMAT_PCM_FLOAT",
};
static const int32_t audioFormatEnum[] = {
        AAUDIO_FORMAT_INVALID,
        AAUDIO_FORMAT_UNSPECIFIED,
        AAUDIO_FORMAT_PCM_I16,
        AAUDIO_FORMAT_PCM_FLOAT,
};
static const int32_t audioFormatCount = sizeof(audioFormatEnum)/
                                        sizeof(audioFormatEnum[0]);
const char* FormatToString(aaudio_format_t format) {
    for (int32_t i = 0; i < audioFormatCount; ++i) {
        if (audioFormatEnum[i] == format)
            return audioFormatStr[i];
    }
    return "UNKNOW_AUDIO_FORMAT";
}
void printAudioStreamInfo(const AAudioStream * stream) {
#define STREAM_CALL(c) AAudioStream_##c((AAudioStream*)stream)
    LOGE("StreamID: %p", stream);

    LOGE("缓冲区容量: %d", STREAM_CALL(getBufferCapacityInFrames));
    LOGE("缓冲区大小: %d", STREAM_CALL(getBufferSizeInFrames));
    LOGE("一次读写的帧数: %d", STREAM_CALL(getFramesPerBurst));
    //欠载和过载在官方文档的描述里，大致是欠载-消费者消费的速度大于生产的速度，过载就是生产的速度大于消费的速度
    LOGE("欠载或过载的数量: %d", STREAM_CALL(getXRunCount));
    LOGE("采样率: %d", STREAM_CALL(getSampleRate));
    LOGE("声道布局: %d", STREAM_CALL(getChannelCount));
    LOGE("音频设备id: %d", STREAM_CALL(getDeviceId));
    LOGE("音频格式: %s",  FormatToString(STREAM_CALL(getFormat)));
    LOGE("流的共享模式: %s", (STREAM_CALL(getSharingMode)) == AAUDIO_SHARING_MODE_EXCLUSIVE ?
                            "独占" : "共享");

    aaudio_performance_mode_t perfMode = STREAM_CALL(getPerformanceMode);
    std::string perfModeDescription;
    switch (perfMode){
        case AAUDIO_PERFORMANCE_MODE_NONE:
            perfModeDescription = "NONE";
            break;
        case AAUDIO_PERFORMANCE_MODE_LOW_LATENCY:
            perfModeDescription = "LOW_LATENCY";
            break;
        case AAUDIO_PERFORMANCE_MODE_POWER_SAVING:
            perfModeDescription = "POWER_SAVING";
            break;
        default:
            perfModeDescription = "UNKNOWN";
            break;
    }
    LOGE("PerformanceMode: %s", perfModeDescription.c_str());

    aaudio_direction_t  dir = STREAM_CALL(getDirection);
    LOGE("Direction: %s", (dir == AAUDIO_DIRECTION_OUTPUT ? "OUTPUT" : "INPUT"));
    if (dir == AAUDIO_DIRECTION_OUTPUT) {
        LOGE("FramesReadByDevice: %d", (int32_t)STREAM_CALL(getFramesRead));
        LOGE("FramesWriteByApp: %d", (int32_t)STREAM_CALL(getFramesWritten));
    } else {
        LOGE("FramesReadByApp: %d", (int32_t)STREAM_CALL(getFramesRead));
        LOGE("FramesWriteByDevice: %d", (int32_t)STREAM_CALL(getFramesWritten));
    }
#undef STREAM_CALL
}
aaudio_result_t
dataCallBack(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames) {
    AudioPlayer *audioPlayer = static_cast<AudioPlayer *>(userData);

}

void AudioPlayer::create() {
    AAudioStreamBuilder *builder;
    result = AAudio_createStreamBuilder(&builder);
    if (result != AAUDIO_OK) {
        LOGE("初始化构造器失败，errorCode:%s", AAudio_convertResultToText(result));
        return;
    }
    if (builder != nullptr) {
        AAudioStreamBuilder_setDeviceId(builder, AAUDIO_UNSPECIFIED);
        AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
        AAudioStreamBuilder_setChannelCount(builder, 2);
        AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
        AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
        AAudioStreamBuilder_setDataCallback(builder, ::dataCallBack, this);
    }

    AAudioStream *audioStream;
    result = AAudioStreamBuilder_openStream(builder, &audioStream);
    if (result != AAUDIO_OK) {
        LOGE("无法创建音频流 errorCode: %s", AAudio_convertResultToText(result));
        return;
    }
    if (audioStream != nullptr) {
        sampleRate = AAudioStream_getSampleRate(audioStream);
        //获取读写一次所需要的帧数
        bufSizeInFrames = AAudioStream_getBufferSizeInFrames(audioStream);
        //设置缓冲区的大小
        AAudioStream_setBufferSizeInFrames(audioStream, bufSizeInFrames);

    }

}

