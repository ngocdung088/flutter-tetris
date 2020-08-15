#include <cassert>
#include <cstdio>
#include "LowLatencyAudio.h"
#include "AudioPlayer.h"

#define LOG_TAG "AudioPool"

#define _CHECK(result) assert(result == SL_RESULT_SUCCESS)

AudioPlayer::AudioPlayer(SLEngineItf engineEngine, SLObjectItf outputMixObject) {
    this->engineEngine = engineEngine;
    this->outputMixObject = outputMixObject;
}


void AudioPlayer::play() {
    if (!prepared) {
        return;
    }
    if (seekItf != nullptr) {
        (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PAUSED);
        (*seekItf)->SetPosition(seekItf, 0, SL_SEEKMODE_FAST);
    }
    if (playItf != nullptr) {
        (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);
    }
}

void AudioPlayer::pause() {
//    if (!prepared) {
//        return;
//    }
//    if (playItf != nullptr) {
//        (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PAUSED);
//    }
}


void AudioPlayer::prepare(const char *url) {
    SLDataLocator_URI locatorUri = {SL_DATALOCATOR_URI, (SLchar *) url};
    SLDataFormat_MIME formatMime = {SL_DATAFORMAT_MIME, nullptr, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&locatorUri, &formatMime};
    prepare(audioSrc);
}

void AudioPlayer::prepareFd(const int fd, off_t start, off_t length) {
    SLDataLocator_AndroidFD locatorUri = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME formatMime = {SL_DATAFORMAT_MIME, nullptr, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&locatorUri, &formatMime};
    prepare(audioSrc);
}


void AudioPlayer::prepare(SLDataSource audioSrc) {
    if (prepared) {
        return;
    }
    SLresult result;
    SLDataLocator_OutputMix locatorOutputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSink = {&locatorOutputMix, nullptr};

    const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &audioSrc,
                                                &audioSink, 3, ids, req);
    assert(SL_RESULT_SUCCESS == result);

    result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGD("can not create realize output mix object,error: %d", result);
        (*playerObject)->Destroy(playerObject);
        return;
    }

    result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playItf);
    assert(result == SL_RESULT_SUCCESS);

    result = (*playerObject)->GetInterface(playerObject, SL_IID_SEEK, &seekItf);
    _CHECK(result);

    result = (*playerObject)->GetInterface(playerObject, SL_IID_MUTESOLO, &muteSoloItf);
    _CHECK(result);

//    (*seekItf)->SetLoop(seekItf, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);

    prepared = true;
}


AudioPlayer::~AudioPlayer() {
    if (playerObject != nullptr) {
        (*playerObject)->Destroy(playerObject);
    }
}