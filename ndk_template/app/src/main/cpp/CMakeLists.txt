# Set the minimum version of CMake required for building
# the native library
cmake_minimum_required(VERSION 3.22.1)

# Defines the name of the native library
project (native-lib)

# Specifies the library name, specify whether it is STATIC
# or SHARED and provides the relative path to the source code
# native-lib will be build as SHARED library so the file format
# is "libnative-lib.so"
add_library (
    native-lib
    SHARED
    ${CMAKE_SOURCE_DIR}/native-lib.cpp
)

add_library (
    app-glue
    STATIC
    ${NDK_HOME}/sources/android/native_app_glue/android_native_app_glue.c
)

# Specify the relative path to the native library's header files
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# Define the name of the path variable that stores the location
# of the NDK library and specify the name of the NDK library
# that CMake needs to locate.
find_library (
    log-lib
    log
)

# Links the target library (native-lib) against one or more
# native libraries
target_link_libraries(
    native-lib
    app-glue
    ${log-lib}
)



