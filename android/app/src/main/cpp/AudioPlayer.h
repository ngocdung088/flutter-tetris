
class AudioPlayer {
public:
    AudioPlayer(SLEngineItf engineEngine, SLObjectItf outputMixObject);

    void play();

    void pause();

    void prepare(const char *url);

    void prepareFd(const int fd, off_t start, off_t length);

    ~AudioPlayer();

private:
    SLEngineItf engineEngine;
    SLObjectItf outputMixObject;

    SLObjectItf playerObject{};

    SLPlayItf playItf = nullptr;
    SLSeekItf seekItf = nullptr;
    SLMuteSoloItf muteSoloItf{};

    void prepare(SLDataSource source);


    bool prepared = false;
};

