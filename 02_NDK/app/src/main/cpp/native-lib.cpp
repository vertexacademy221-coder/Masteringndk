#include <jni.h>
#include <android/log.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <curl/curl.h>

#include <cmath>
#include <vector>


extern "C"
{
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
}

extern "C"
{
#include <ft2build.h>
#include FT_FREETYPE_H // Équivaut à inclure <freetype/freetype.h>
}

#define LOG_TAG "NDK-AudioEngine"
#define LOGI(...)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


/* ======= OpenAL Gloal variables ====== */

ALCdevice   *openal_device = nullptr;
ALCcontext  *openal_ctx = nullptr;
ALuint      audio_buffer = 0;
ALuint      audio_source = 0;

/* ============== Function prototype =======*/

void initCurl();
size_t MemoryCallback( void* P, size_t Size, size_t Num, void* );
void initFreeTypeExample();

/* ============ Function definition ======= */

void initFreeTypeExample() {
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    
    if (error) {
        // Erreur d'initialisation
    }
    
    // Votre code pour charger des polices (.ttf) ici...
    
    FT_Done_FreeType(library);
}

size_t MemoryCallback( void* P, size_t Size, size_t Num, void* )
{
    if ( !P ) {
        if (Num == 0){
            LOGE("There is no data");
        }
        return 0;
    }
    LOGI( "%s\n", (char*)P );
    return 1;
}


void initCurl(){
    LOGI("Initializing CURL");

    CURL* Curl = curl_easy_init();
    curl_easy_setopt( Curl, CURLOPT_URL, "http://example.com/" );
    curl_easy_setopt( Curl, CURLOPT_FOLLOWLOCATION, 1 );
    curl_easy_setopt( Curl, CURLOPT_FAILONERROR, true );
    curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, &MemoryCallback );
    curl_easy_setopt( Curl, CURLOPT_WRITEDATA, 0 );
    curl_easy_perform( Curl );
    curl_easy_cleanup( Curl );
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_vertexacademy_masteringndk_AudioEngine_initOpenAL(JNIEnv *env, jobject thiz)
{
    // Init CURL
    initCurl();
    
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