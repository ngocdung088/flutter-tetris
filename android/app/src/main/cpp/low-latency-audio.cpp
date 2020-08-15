#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <cassert>
#include <cstdint>
#include "LowLatencyAudio.h"
#include "AudioPlayer.h"

#define LOG_TAG "LowLatencyAudio"

int createEngineIfNeed();

void shutdownEngineIfNoPlayer();

static AAssetManager *sAAssetManager;

extern "C"
JNIEXPORT void JNICALL
Java_tech_soit_flutter_1tetris_MainActivity_initJni(JNIEnv *env, jclass clazz,
                                                    jobject asset_manager) {
    sAAssetManager = AAssetManager_fromJava(env, asset_manager);
}

static SLObjectItf engineObject;
static SLObjectItf outputMixObject;
static SLEngineItf engineEngine;

static int playerCount;

DARTEXPORT
AudioPlayer *createPlayer() {
    createEngineIfNeed();
    if (engineEngine == nullptr) {
        return nullptr;
    }
    auto *audioPlayer = new AudioPlayer(engineEngine, outputMixObject);
    playerCount++;
    LOGD("create player : %p", audioPlayer);
    return audioPlayer;
}

DARTEXPORT
void releasePlayer(AudioPlayer *player) {
    if (player != nullptr) {
        playerCount--;
        delete player;
    }
    shutdownEngineIfNoPlayer();
}


DARTEXPORT
void loadAsset(AudioPlayer *player, char *filename) {
    LOGD("load assets: player: %p file: %s", player, filename);
    if (player == nullptr) {
        return;
    }
    if (sAAssetManager == nullptr) {
        LOGD("can not load assets ,because AssetManager did not init.");
        return;
    }
    auto asset = AAssetManager_open(sAAssetManager, filename, AASSET_MODE_UNKNOWN);
    if (!asset) {
        LOGD("can not open asset: %s", filename);
        return;
    }
    off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);
    if (!fd) {
        LOGD("can not open fd for asset : %s", filename);
        return;
    }
    player->prepareFd(fd, start, length);
}

DARTEXPORT
void play(AudioPlayer *player) {
    player->play();
}

DARTEXPORT
void pause(AudioPlayer *player) {
    player->pause();
}

int createEngineIfNeed() {
    if (engineEngine != nullptr) {
        return -1;
    }
    SLresult result;

    result = slCreateEngine(&engineObject, 0, nullptr,
                            0, nullptr, nullptr);
    assert(result == SL_RESULT_SUCCESS);

    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);

    {
        const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
        const SLboolean req[1] = {SL_BOOLEAN_FALSE};
        result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
        assert(result == SL_RESULT_SUCCESS);
    }

    result = (*outputMixObject)->Realize(outputMixObject, false);
    assert(result == SL_RESULT_SUCCESS);

    SLEnvironmentalReverbItf outputMixEnvironmentalReverb;
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);
    if (result == SL_RESULT_SUCCESS) {
        const SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        (void) result;
    }
    return 0;
}

void shutdownEngineIfNoPlayer() {
    if (playerCount > 0) {
        return;
    }
    if (outputMixObject != nullptr) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = nullptr;
    }
    if (engineObject != nullptr) {
        (*engineObject)->Destroy(engineObject);
        engineObject = nullptr;
        engineEngine = nullptr;
    }
}

