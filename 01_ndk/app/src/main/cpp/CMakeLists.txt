cmake_minimum_required(VERSION 3.22.1)

project(native-lib)

# This command find a file named native-lib.cpp
# create a shared library named libnative-lib.so and
# package it with apk file executable

add_library(
        # specify the name of the library
        native-lib

        # Sets the library as a shared library.
        # Gradle will automatically package it with APK file
        SHARED

        # Provide relative path to the source file
        ${CMAKE_SOURCE_DIR}/native-lib.cpp
)

# This command specify where to find header files
include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

# This command locate NDK library and store its path
# as a variable

find_library(
        # Defines the name of the path variable that
        # stores the location of the NDK library
        log-lib

        # Specifies the name of the NDK library
        # that CMake needs to locate
        log
)

# In order to native-lib to call functions in log library
# we need to link the libraries using the target_link_libraries
# command

target_link_libraries(
    # Specifies the target library
    native-lib

    # Links to log library with
    # the specified target library
    ${log-lib}
)
