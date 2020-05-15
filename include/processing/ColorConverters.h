/**
 * @file ColorConverters.h
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#ifndef _PROCESSING_COLORCONVERTERS_H_
#define _PROCESSING_COLORCONVERTERS_H_

#include <iostream>
#include <memory>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

namespace pobr {
    enum ColorConversionType {
        BGR2HSV
    };

    cv::Mat cvtColor(const cv::Mat& image, ColorConversionType type);
    cv::Vec3b cvtColor(const cv::Vec3b& vec, ColorConversionType type);
}

class ColorConverter {
public:
    static std::shared_ptr<ColorConverter> makeColorConverter(pobr::ColorConversionType type);
    virtual cv::Mat convert(const cv::Mat& image) = 0;
    virtual cv::Vec3b convert(const cv::Vec3b& vec) = 0;
    virtual ~ColorConverter() {}
};

class ColorPixelConverter {
public:
    virtual cv::Vec3b convert(const cv::Vec3b& org) = 0;
    virtual ~ColorPixelConverter() {};
};

class BGR2HSVConverter : public ColorConverter {
public:
    BGR2HSVConverter() {}
    cv::Mat convert(const cv::Mat& image) override;
    cv::Vec3b convert(const cv::Vec3b& vec) override;

private:
    class BGR2HSVPixelConverter : public ColorPixelConverter {
    public:
        cv::Vec3b convert(const cv::Vec3b& bgr) override;    
    } pixelConverter_;
};






#endif // _PROCESSING_COLORCONVERTERS_H_