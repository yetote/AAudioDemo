

#ifndef AAUDIODEMO_AUDIOPLAYER_H
#define AAUDIODEMO_AUDIOPLAYER_H


#include <aaudio/AAudio.h>
#include <string>
#include "../util/LogUtil.h"
#include "../util/CallBack.h"
#include <thread>

#define LOG_TAG "audioPlayer"
#define AAUDIO_NANOS_PER_MILLISECOND 1000000L

class AudioPlayer {
public:
    AudioPlayer(const char *path, CallBack *callBack);

    ~AudioPlayer();


    aaudio_data_callback_result_t dataCallback(AAudioStream *stream,
                                               void *audioData, int32_t numFrames);

    FILE *file;
    AAudioStream *playerStream, *readStream;
private:
    aaudio_result_t result;
    int32_t sampleRate;
    int32_t bufSizeInFrames;
    CallBack *callBack;
    char *dataPos;

    void writeData();

    void createStream(AAudioStream *stream);

    void createPlayerBuilder();

    void createReadBuilder();

};


#endif //AAUDIODEMO_AUDIOPLAYER_H
