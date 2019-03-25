

#ifndef AAUDIODEMO_AUDIOPLAYER_H
#define AAUDIODEMO_AUDIOPLAYER_H

#define LOG_TAG "audioPlayer"

#include <aaudio/AAudio.h>
#include <string>
#include "../util/LogUtil.h"
class AudioPlayer {
public:
    AudioPlayer();

    ~AudioPlayer();


private:
    aaudio_result_t result;
    int32_t sampleRate;
    int32_t bufSizeInFrames;

    aaudio_data_callback_result_t dataCallback(AAudioStream *stream, void *userData,
                                               void *audioData, int32_t numFrames);

    void create();

};


#endif //AAUDIODEMO_AUDIOPLAYER_H
