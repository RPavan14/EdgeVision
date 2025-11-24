#include <jni.h>
#include <string>
#include <android/log.h>
#include "opencv_processor.h"
#include "gl_renderer.h"

#define LOG_TAG "EdgeVision-Native"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static OpenCVProcessor* processor = nullptr;
static GLRenderer* glRenderer = nullptr;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_edgevision_pro_MainActivity_nativeInitialize(JNIEnv *env, jobject thiz, jint width, jint height) {
    LOGI("Initializing native components: %dx%d", width, height);
    
    try {
        processor = new OpenCVProcessor(width, height);
        glRenderer = new GLRenderer();
        
        if (!processor->initialize()) {
            LOGE("Failed to initialize OpenCV processor");
            return JNI_FALSE;
        }
        
        if (!glRenderer->initialize()) {
            LOGE("Failed to initialize GL renderer");
            return JNI_FALSE;
        }
        
        LOGI("Native components initialized successfully");
        return JNI_TRUE;
    } catch (const std::exception& e) {
        LOGE("Exception during initialization: %s", e.what());
        return JNI_FALSE;
    }
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_edgevision_pro_MainActivity_nativeProcessFrame(JNIEnv *env, jobject thiz, jbyteArray data, jint size) {
    if (!processor) {
        LOGE("Processor not initialized");
        return nullptr;
    }
    
    jbyte* inputData = env->GetByteArrayElements(data, nullptr);
    if (!inputData) {
        LOGE("Failed to get input data");
        return nullptr;
    }
    
    try {
        std::vector<uint8_t> processedData = processor->processFrame(
            reinterpret_cast<uint8_t*>(inputData), size
        );
        
        env->ReleaseByteArrayElements(data, inputData, JNI_ABORT);
        
        if (processedData.empty()) {
            LOGE("Processing returned empty data");
            return nullptr;
        }
        
        jbyteArray result = env->NewByteArray(processedData.size());
        env->SetByteArrayRegion(result, 0, processedData.size(), 
                               reinterpret_cast<const jbyte*>(processedData.data()));
        
        return result;
    } catch (const std::exception& e) {
        LOGE("Exception during frame processing: %s", e.what());
        env->ReleaseByteArrayElements(data, inputData, JNI_ABORT);
        return nullptr;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_edgevision_pro_MainActivity_nativeSetProcessingMode(JNIEnv *env, jobject thiz, jboolean enabled) {
    if (processor) {
        processor->setProcessingEnabled(enabled);
        LOGI("Processing mode set to: %s", enabled ? "enabled" : "disabled");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_edgevision_pro_MainActivity_nativeUpdateSize(JNIEnv *env, jobject thiz, jint width, jint height) {
    if (processor) {
        processor->updateSize(width, height);
        LOGI("Size updated to: %dx%d", width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_edgevision_pro_MainActivity_nativeDestroy(JNIEnv *env, jobject thiz) {
    LOGI("Destroying native components");
    
    if (processor) {
        delete processor;
        processor = nullptr;
    }
    
    if (glRenderer) {
        delete glRenderer;
        glRenderer = nullptr;
    }
    
    LOGI("Native components destroyed");
}
