/**
 * @file ColorConverters.cpp
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#include "../../include/processing/ColorConverters.h"

cv::Vec3b BGR2HSVConverter::BGR2HSVPixelConverter::convert(const cv::Vec3b& brg) {
    double b = static_cast<double>(brg[0]) / UCHAR_MAX;
    double r = static_cast<double>(brg[1]) / UCHAR_MAX;
    double g = static_cast<double>(brg[2]) / UCHAR_MAX;

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

