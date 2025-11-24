cmake_minimum_required(VERSION 3.18.1)

project("edgevision")

# OpenCV configuration
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/OpenCV-android-sdk/sdk/native/jni")
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(src/main/cpp)
include_directories(../jni)

# Add native library
add_library(
    edgevision
    SHARED
    src/main/cpp/native-lib.cpp
    src/main/cpp/opencv_processor.cpp
    src/main/cpp/gl_renderer.cpp
    src/main/cpp/jni_bridge.cpp
)

# Link libraries
target_link_libraries(
    edgevision
    ${OpenCV_LIBS}
    android
    log
    GLESv2
    EGL
)
