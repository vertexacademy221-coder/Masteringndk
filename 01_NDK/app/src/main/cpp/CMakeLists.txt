cmake_minimum_required(VERSION 3.22.1)

# the name of the project we are going to build 
project (native-lib)

# This commands create a shared library (libnative-lib.so) from 
# source code file native-lib.cpp. Then this shared library is
# package into our Apk file.
add_library(
    native-lib
    SHARED
    ${CMAKE_SOURCE_DIR}/native-lib.cpp
)

# This command specifies the include directory
# for the headers functions used in the native library
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)


find_library(
    # Define the name of the path variable for 
    # the library used in the native-lib
    log-lib

    # Specifies the library to get the location
    log
)


target_link_libraries(
    # Target library
    native-lib
    # Links library with the target library
    ${log-lib}
)