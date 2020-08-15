cmake_minimum_required(VERSION 3.10.2)


include(LowLatencyAudio.h)

add_library(
        low-latency-audio

        SHARED

        low-latency-audio.cpp
        AudioPlayer.cpp
)

find_library(
        log-lib
        log
)

target_link_libraries(
        low-latency-audio
        android
        OpenSLES
        ${log-lib}
)