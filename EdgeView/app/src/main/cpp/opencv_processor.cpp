#include "opencv_processor.h"
#include <android/log.h>

#define LOG_TAG "OpenCVProcessor"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

OpenCVProcessor::OpenCVProcessor(int width, int height) 
    : frameWidth(width), frameHeight(height), processingEnabled(true) {
    LOGI("OpenCV Processor created: %dx%d", width, height);
}

OpenCVProcessor::~OpenCVProcessor() {
    LOGI("OpenCV Processor destroyed");
}

bool OpenCVProcessor::initialize() {
    try {
        // Initialize OpenCV matrices
        inputMat = cv::Mat::zeros(frameHeight, frameWidth, CV_8UC4);
        grayMat = cv::Mat::zeros(frameHeight, frameWidth, CV_8UC1);
        edgeMat = cv::Mat::zeros(frameHeight, frameWidth, CV_8UC1);
        outputMat = cv::Mat::zeros(frameHeight, frameWidth, CV_8UC4);
        
        LOGI("OpenCV matrices initialized successfully");
        return true;
    } catch (const cv::Exception& e) {
        LOGE("OpenCV initialization error: %s", e.what());
        return false;
    }
}

std::vector<uint8_t> OpenCVProcessor::processFrame(const uint8_t* data, int size) {
    if (!data || size <= 0) {
        LOGE("Invalid input data");
        return {};
    }
    
    try {
        // Copy input data to OpenCV Mat
        std::memcpy(inputMat.data, data, std::min(size, frameWidth * frameHeight * 4));
        
        if (processingEnabled) {
            // Convert RGBA to Grayscale
            cv::cvtColor(inputMat, grayMat, cv::COLOR_RGBA2GRAY);
            
            // Apply Gaussian blur to reduce noise
            cv::GaussianBlur(grayMat, grayMat, cv::Size(5, 5), 1.4);
            
            // Apply Canny edge detection
            cv::Canny(grayMat, edgeMat, 50, 150, 3);
            
            // Convert edges back to RGBA for display
            cv::cvtColor(edgeMat, outputMat, cv::COLOR_GRAY2RGBA);
            
            // Optional: Add some color to edges
            for (int y = 0; y < frameHeight; ++y) {
                for (int x = 0; x < frameWidth; ++x) {
                    cv::Vec4b& pixel = outputMat.at<cv::Vec4b>(y, x);
                    if (pixel[0] > 0) { // If edge detected
                        pixel[0] = 0;     // Blue
                        pixel[1] = 255;   // Green
                        pixel[2] = 255;   // Red
                        pixel[3] = 255;   // Alpha
                    }
                }
            }
        } else {
            // Pass through original frame
            inputMat.copyTo(outputMat);
        }
        
        // Convert result to vector
        std::vector<uint8_t> result(outputMat.data, outputMat.data + outputMat.total() * outputMat.elemSize());
        return result;
        
    } catch (const cv::Exception& e) {
        LOGE("OpenCV processing error: %s", e.what());
        return {};
    }
}

void OpenCVProcessor::setProcessingEnabled(bool enabled) {
    processingEnabled = enabled;
    LOGI("Processing %s", enabled ? "enabled" : "disabled");
}

void OpenCVProcessor::updateSize(int width, int height) {
    if (width != frameWidth || height != frameHeight) {
        frameWidth = width;
        frameHeight = height;
        
        // Reinitialize matrices with new size
        initialize();
        
        LOGI("Size updated to: %dx%d", width, height);
    }
}

void OpenCVProcessor::setCannyThresholds(double lowThreshold, double highThreshold) {
    cannyLowThreshold = lowThreshold;
    cannyHighThreshold = highThreshold;
    LOGI("Canny thresholds updated: %.1f, %.1f", lowThreshold, highThreshold);
}
