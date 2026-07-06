cmake_minimum_required(VERSION 3.22.1)

project(native-lib)

# =========================================================
# OPENAL ANDROID CONFIGURATION
# =========================================================

set(ALSOFT_EMBED_HRTF_DATA ON CACHE BOOL "Embed HRTF data" FORCE) # Pratique pour l'audio 3D au casque
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)        # Évite de compiler les exécutables de test
set(ALSOFT_UTILS OFF CACHE BOOL "Build utilities" FORCE)          # Évite de compiler les outils en ligne de commande
set(LIBTYPE "STATIC" CACHE STRING "Build type" FORCE)             # On force la compilation en bibliothèque STATIQUE


# 2. Include subdirectories for static libraries
add_subdirectory(third-party/openal)


# =========================================================
# OPENSSL ANDROID CONFIGURATION
# =========================================================

set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/third-party/openssl")

add_library(crypto_static STATIC IMPORTED)
set_target_properties(crypto_static PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_ROOT_DIR}/libs/${ANDROID_ABI}/libcrypto.a"
)

add_library(ssl_static STATIC IMPORTED)
set_target_properties(ssl_static PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_ROOT_DIR}/libs/${ANDROID_ABI}/libssl.a"
)

include_directories("${OPENSSL_ROOT_DIR}/include")

# Create Alias library name for libcurl
add_library(OpenSSL::Crypto STATIC IMPORTED)

# ${ANDROID_ABI} is automatically provide by Gradle
set_target_properties(OpenSSL::Crypto PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_ROOT_DIR}/libs/${ANDROID_ABI}/libcrypto.a"
    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_ROOT_DIR}/include"
)

add_library(OpenSSL::SSL STATIC IMPORTED)
set_target_properties(OpenSSL::SSL PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_ROOT_DIR}/libs/${ANDROID_ABI}/libssl.a"
    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_ROOT_DIR}/include"
)

# On force CMake à croire que find_package(OpenSSL) a déjà réussi
set(OPENSSL_FOUND TRUE CACHE BOOL "" FORCE)
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include" CACHE PATH "" FORCE)
set(OPENSSL_LIBRARIES OpenSSL::SSL OpenSSL::Crypto CACHE STRING "" FORCE)


# =========================================================
# LIBCURL CONFIGURATION WITH SSL
# =========================================================

# 1. Configure libcurl before hand
# Here we force the build only for static libraries
set (BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set (BUILD_STATIC_LIBS ON CACHE BOOL "Build static librairies" FORCE)

# Search for curl executable, tests and documentation
set (BUILD_CURL_EXE OFF CACHE BOOL "Build curl executable" FORCE)
set (BUILD_TESTING OFF CACHE BOOL "Build tests" FORCE)
set (ENABLE_MANUAL OFF CACHE BOOL "Build manual" FORCE)

# Choose options for curl
set (HTTP_ONLY OFF CACHE BOOL "Disable all protocols except HTTP/S" FORCE)       
set (CURL_DISABLE_VERBOSE_STRINGS ON CACHE BOOL "Disable verbos strings" FORCE)

# By default curl use Schannel or OpenSSL. If we dont need HTTPS, we can deactivate SSL
set (CURL_USE_OPENSSL ON CACHE BOOL "Use OpenSSL" FORCE)
set (CURL_USE_LIBPSL OFF CACHE BOOL "Use libPsl" FORCE)

# Add libcurl.a to our build
set (CURL_INSTALL_DISABLE ON CACHE BOOL "Disable curl install targets" FORCE)

# On passe les variables classiques attendues par les vieux scripts de curl
set(OPENSSL_CRYPTO_LIBRARY crypto_static CACHE FILEPATH "" FORCE)
set(OPENSSL_SSL_LIBRARY ssl_static CACHE FILEPATH "" FORCE)

add_subdirectory(third-party/curl)

# ==================================================
# FREETYPE CONFIGURATION
# ==================================================

set(DISABLE_STATIC_LIBS OFF CACHE BOOL "Build static library" FORCE)
set(DISABLE_SHARED_LIBS ON CACHE BOOL "Build shared library" FORCE)
set(FT_WITH_ZLIB ON CACHE BOOL "Compile FreeType with system ZLib" FORCE)
set(FT_WITH_BZIP2 OFF CACHE BOOL "Compile FreeType with BZip2" FORCE)
set(FT_WITH_PNG OFF CACHE BOOL "Compile FreeType with PNG" FORCE)
set(FT_WITH_HARFBUZZ OFF CACHE BOOL "Compile FreeType with HarfBuzz" FORCE)

add_subdirectory(third-party/freetype)


# Specify the name of target native libray. 
# We want to build a shared library (libnative-lib.so)
# and the code source is in file native-lib.cpp.
# The libnative-lib.so will be package inside our APK file
add_library(
    native-lib
    SHARED
    "${CMAKE_SOURCE_DIR}/native-lib.cpp"
    "${CMAKE_SOURCE_DIR}/FT_Utils.cpp"
    "${CMAKE_SOURCE_DIR}/FIEngine.cpp" 
)

# For the header files of our native library
# Go in src/main/cpp/include
include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/third-party/curl/include"
    "${CMAKE_SOURCE_DIR}/third-party/stb"
    "${CMAKE_SOURCE_DIR}/third-party/freetype/include"
)

# Specify the static NDK library to locate
# and store its location inside path variable named
# app-glue
add_library(
    app-glue
    STATIC
    "${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c"
)

# Specify include directories for NDK library header file
target_include_directories(
    app-glue
    PUBLIC
    "${ANDROID_NDK}/sources/android/native_app_glue"
)


# Defines the variable path name of the NDK library and
# Get the location of the NDK library and stores it inside
# that variable.
find_library(
    log-lib
    log
)


find_library(
    android-lib 
    android
)

# Links NDK library against our Native library
target_link_libraries(
    native-lib
    app-glue
    OpenAL
    freetype
    libcurl
    ssl_static
    crypto_static
    ${log-lib}
    ${android-lib}
)
