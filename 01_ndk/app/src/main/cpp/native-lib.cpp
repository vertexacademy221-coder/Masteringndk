#include <stdlib.h>
#include <jni.h>
#include <android/log.h>

#define TAG "NDKApp"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))

extern "C"
{
    JNIEXPORT void JNICALL
    Java_vertexacademy_com_masteringndk_App1Activity_onCreateNative (
        JNIEnv *env, jobject obj)
        {
            LOGI ("Hello Android NDK!");
        }
}