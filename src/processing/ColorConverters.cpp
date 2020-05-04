/**
 * @file ColorConverters.cpp
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#include "../../include/processing/ColorConverters.h"

namespace pobr {
    cv::Mat cvtColor(cv::Mat& image, ColorConversionType type) {
        auto converter = ColorConverter::makeColorConverter(type);
        return converter->convert(image);
    }
}

std::shared_ptr<ColorConverter> ColorConverter::makeColorConverter(pobr::ColorConversionType type) {
    std::shared_ptr<ColorConverter> cvt;
    switch (type) {
    case pobr::BRG2HSV:
        cvt = std::make_shared<BGR2HSVConverter>();
        break;
    default:
        throw(std::out_of_range("Color conversion type not supported"));
    }
    return cvt;
}

cv::Vec3b BGR2HSVConverter::BGR2HSVPixelConverter::convert(const cv::Vec3b& bgr) {
    double b = static_cast<double>(bgr[0]) / UCHAR_MAX;
    double g = static_cast<double>(bgr[1]) / UCHAR_MAX;
    double r = static_cast<double>(bgr[2]) / UCHAR_MAX;

    double cmax = std::max(b, std::max(g, r));
    double cmin = std::min(b, std::min(g, r));
    double diff = cmax - cmin;
    int h = -1, s = -1;
    int v = cmax * UCHAR_MAX;

    if (cmax == cmin) {
        h = 0;
    }
    else if (cmax == r) {
        h = static_cast<int>(round(((60 * static_cast<int>((g - b) / diff) + 360) % 360) / 2));
    }
    else if (cmax == g) {
        h = static_cast<int>(round(((60 * static_cast<int>((b - r) / diff) + 120) % 360) / 2));
    }
    else if (cmax == b) {
        h = static_cast<int>(round(((60 * static_cast<int>((r - g) / diff) + 240) % 360) / 2));
    }
    if (cmax == 0) {
        s = 0;
    }
    else {
        s = (diff / cmax) * UCHAR_MAX;
    }

    return cv::Vec3b(h, s, v);
}

cv::Mat BGR2HSVConverter::convert(cv::Mat& imageBRG) {
    cv::Mat imageHSV = imageBRG.clone();
    imageHSV.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int position[]) {
        px = pixelConverter_.convert(px);
    });
    return imageHSV;
}

