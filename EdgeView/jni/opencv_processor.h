#ifndef OPENCV_PROCESSOR_H
#define OPENCV_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdint>

class OpenCVProcessor {
public:
    OpenCVProcessor(int width, int height);
    ~OpenCVProcessor();
    
    bool initialize();
    std::vector<uint8_t> processFrame(const uint8_t* data, int size);
    void setProcessingEnabled(bool enabled);
    void updateSize(int width, int height);
    void setCannyThresholds(double lowThreshold, double highThreshold);
    
private:
    int frameWidth;
    int frameHeight;
    bool processingEnabled;
    
    double cannyLowThreshold = 50.0;
    double cannyHighThreshold = 150.0;
    
    cv::Mat inputMat;
    cv::Mat grayMat;
    cv::Mat edgeMat;
    cv::Mat outputMat;
};

#endif // OPENCV_PROCESSOR_H
