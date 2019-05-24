//
// Created by ether on 2019/3/25.
//


#include <zconf.h>
#include "AudioPlayer.h"

static const char *audioFormatStr[] = {
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
static const int32_t audioFormatCount = sizeof(audioFormatEnum) /
                                        sizeof(audioFormatEnum[0]);

const char *FormatToString(aaudio_format_t format) {
    for (int32_t i = 0; i < audioFormatCount; ++i) {
        if (audioFormatEnum[i] == format)
            return audioFormatStr[i];
    }
    return "UNKNOW_AUDIO_FORMAT";
}

void printAudioStreamInfo(const AAudioStream *stream) {
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
    LOGE("音频格式: %s", FormatToString(STREAM_CALL(getFormat)));
    LOGE("流的共享模式: %s", (STREAM_CALL(getSharingMode)) == AAUDIO_SHARING_MODE_EXCLUSIVE ?
                       "独占" : "共享");

    aaudio_performance_mode_t perfMode = STREAM_CALL(getPerformanceMode);
    std::string perfModeDescription;
    switch (perfMode) {
        case AAUDIO_PERFORMANCE_MODE_NONE:
            perfModeDescription = "默认模式";
            break;
        case AAUDIO_PERFORMANCE_MODE_LOW_LATENCY:
            perfModeDescription = "低延迟";
            break;
        case AAUDIO_PERFORMANCE_MODE_POWER_SAVING:
            perfModeDescription = "节能";
            break;
        default:
            perfModeDescription = "UNKNOWN";
            break;
    }
    LOGE("性能模式: %s", perfModeDescription.c_str());

    aaudio_direction_t dir = STREAM_CALL(getDirection);
    LOGE("流方向: %s", (dir == AAUDIO_DIRECTION_OUTPUT ? "OUTPUT" : "INPUT"));
    if (dir == AAUDIO_DIRECTION_OUTPUT) {
        LOGE("输出流读取的帧数: %d", (int32_t) STREAM_CALL(getFramesRead));
        LOGE("输出流写入的帧数: %d", (int32_t) STREAM_CALL(getFramesWritten));
    } else {
        LOGE("输入流读取的帧数: %d", (int32_t) STREAM_CALL(getFramesRead));
        LOGE("输入流写入的帧数: %d", (int32_t) STREAM_CALL(getFramesWritten));
    }
#undef STREAM_CALL
}

aaudio_data_callback_result_t dataCallback(AAudioStream *stream, void *userData,
                                           void *audioData, int32_t numFrames) {
    AudioPlayer *audioPlayer = reinterpret_cast<AudioPlayer *>(userData);
    return audioPlayer->dataCallback(stream, audioData, numFrames);
}


aaudio_data_callback_result_t
AudioPlayer::dataCallback(AAudioStream *stream, void *audioData, int32_t numFrames) {
    char *buffer = static_cast<char *>(audioData);
    //AAudio并不建议在回调中进行I/O操作，因为这可能会导致相乘被阻塞
    size_t wFrameCount = fread(buffer, numFrames * 2 * 2, 1, file);
    int j = 0;
    for (int i = 0; i < numFrames * 2 * 2; ++i) {
        if (i % (numFrames * 2 * 2 / 20) == 0) {
            dataPos[j] = buffer[i];
            j++;
        }
    }
    callBack->onDraw(callBack->CHILD_THREAD, dataPos);
    if (wFrameCount < 0) {
        LOGE("写入错误,%d", wFrameCount);

    } else {
        LOGE("读取了%d帧", wFrameCount);
    }
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void AudioPlayer::createPlayerBuilder() {
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
        AAudioStreamBuilder_setSampleRate(builder, 44100);
        AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
        AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
        AAudioStreamBuilder_setDataCallback(builder, ::dataCallback, this);

    } else {
        LOGE("无法获取build对象");
        return;
    }

    result = AAudioStreamBuilder_openStream(builder, &playerStream);
    if (result != AAUDIO_OK) {
        LOGE("无法创建音频流 errorCode: %s", AAudio_convertResultToText(result));
        return;
    }
    if (playerStream != nullptr) {
        sampleRate = AAudioStream_getSampleRate(playerStream);
        //获取读写一次所需要的帧数
        bufSizeInFrames = AAudioStream_getBufferSizeInFrames(playerStream);
        //设置缓冲区的大小
        AAudioStream_setBufferSizeInFrames(playerStream, bufSizeInFrames);
        printAudioStreamInfo(playerStream);

        result = AAudioStream_requestStart(playerStream);
        if (result != AAUDIO_OK) {
            LOGE("请求音频流为打开状态失败，%s", AAudio_convertResultToText(result));
            return;
        }

        aaudio_stream_state_t inputState = AAUDIO_STREAM_STATE_STARTING;
        aaudio_stream_state_t nextState = AAUDIO_STREAM_STATE_UNINITIALIZED;
        int64_t timeoutNanos = 100 * AAUDIO_NANOS_PER_MILLISECOND;

        result = AAudioStream_waitForStateChange(playerStream, inputState, &nextState,
                                                 timeoutNanos);
        if (result != AAUDIO_OK) {
            LOGE("打开流失败. %s", AAudio_convertResultToText(result));
        }
    }
    result = AAudioStreamBuilder_delete(builder);
    if (result != AAUDIO_OK) {
        LOGE("删除构造器失败，%s", AAudio_convertResultToText(result));
        return;
    }
}

AudioPlayer::AudioPlayer(const char *path, CallBack *callBack) {
    file = fopen(path, "rb+");
    dataPos = new char[20];
    this->callBack = callBack;
    createPlayerBuilder();
}




