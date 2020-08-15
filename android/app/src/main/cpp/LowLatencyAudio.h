#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
#include "android/log.h"
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__)

#define DARTEXPORT extern "C"  __attribute__((visibility("default"))) __attribute__((used))
