#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <cmath>
#include <vector>

#include "FIEngine.h"

#define LOG_TAG "NDK-AudioEngine"
#define LOGI(...)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


/* ======= OpenAL Gloal variables ====== */

ALCdevice   *openal_device = nullptr;
ALCcontext  *openal_ctx = nullptr;
ALuint      audio_buffer = 0;
ALuint      audio_source = 0;

/* ============== Function prototype =======*/


extern "C"
JNIEXPORT void JNICALL
Java_com_vertexacademy_masteringndk_AudioEngine_renderFont(JNIEnv *env, jobject thiz, jobject assetManager)
{
    // 1. Récupérer le pointeur natif vers l'AAssetManager d'Android
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    if(mgr == nullptr)
    {
        LOGE("Failed to get AssetManager instance from java");
        return;
    }
    
    // 2. Ouvrir le fichier font.ttf situé dans le dossier assets
    AAsset* asset = AAssetManager_open(mgr, "font.ttf", AASSET_MODE_BUFFER);
    if(asset == nullptr)
    {
        LOGE ("Failed to load \'font.ttf\' ");
        return;
    }

    // 3. Lire la taille du fichier et allouer le conteneur vector
    off_t fileSize = AAsset_getLength(asset);
    std::vector<char> fontBuffer(fileSize);

    // 4. Copier les données binaires du ttf dans notre vector
    int bytesRead = AAsset_read(asset, fontBuffer.data(), fileSize);

    // Fermer l'asset dès que la lecture est terminée
    AAsset_close(asset);

    if (bytesRead <= 0) {
       LOGE("Error while reading font type");
        return;
    }

    LOGI("Font \'font.ttf\' successfully loaded: (%d octets)", bytesRead);

    fiengine::TestFontRendering(fontBuffer);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_vertexacademy_masteringndk_AudioEngine_initOpenAL(JNIEnv *env, jobject thiz)
{

    // 1. Ouvrir le peripherique audio par defaut d'android
    openal_device = alcOpenDevice(nullptr);
    if (!openal_device){
        LOGE("Impossible d'ouvrir le peripherique audio");
        return JNI_FALSE;
    }

    // 2. Créer et activer le contexte audio
    openal_ctx = alcCreateContext(openal_device, nullptr);
    if(!openal_ctx){
        LOGE("Impossible de creer le context pour OpenAL");
        alcCloseDevice(openal_device);
        return JNI_FALSE;
    }

    alcMakeContextCurrent(openal_ctx);
    LOGI("OpenAL initialise avec succes!");

    // 3. Générer un Buffer et une Source
    alGenBuffers(1, &audio_buffer);
    alGenSources(1, &audio_source);

    // 4. Synthèse d'un son (Onde sinusoïdale à 440Hz - Note LA)
    const int sample_rate = 44100;
    const float frequency = 440.0f;
    const int duration_seconds = 2;
    const int num_samples = sample_rate * duration_seconds;

    std::vector<short> target_buffer(num_samples);

    for (int i = 0; i < num_samples; ++i) {
        // Formule de l'onde sinusoïdale standard
        float time = (float)i / sample_rate;
        target_buffer[i] = (short)(32767.0f * sinf(2.0f * M_PI * frequency * time));
    }

    // 5. Remplir le buffer OpenAL avec nos données PCM 16-bit Mono
    alBufferData(audio_buffer, AL_FORMAT_MONO16, target_buffer.data(), 
                target_buffer.size() * sizeof(short), sample_rate);
    
    // 6. Attacher le buffer à la source
    alSourcei(audio_source, AL_BUFFER, audio_buffer);

    return JNI_TRUE;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_vertexacademy_masteringndk_AudioEngine_playTestSound(JNIEnv *env, jobject thiz)
{
    if(audio_source){
        LOGI("Lecture du son test ...");
        alSourcePlay(audio_source);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_vertexacademy_masteringndk_AudioEngine_shutdownOpenAL(JNIEnv *env, jobject thiz)
{
    // Nettoyage des ressources
    if (audio_source) alDeleteSources(1, &audio_source);
    if (audio_buffer) alDeleteBuffers(1, &audio_buffer);
    
    if (openal_ctx) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(openal_ctx);
    }
    if (openal_device) {
        alcCloseDevice(openal_device);
    }
    LOGI("OpenAL arrêté proprement.");
}