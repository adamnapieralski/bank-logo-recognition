/**
 * @file ColorConverters.cpp
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#include "../../include/processing/ColorConverters.h"

namespace pobr {
    cv::Mat cvtColor(const cv::Mat& image, ColorConversionType type) {
        auto converter = ColorConverter::makeColorConverter(type);
        return converter->convert(image);
    }
    
    cv::Vec3b cvtColor(const cv::Vec3b& vec, ColorConversionType type) {
        auto converter = ColorConverter::makeColorConverter(type);
        return converter->convert(vec);
    }

}

std::shared_ptr<ColorConverter> ColorConverter::makeColorConverter(pobr::ColorConversionType type) {
    std::shared_ptr<ColorConverter> cvt;
    switch (type) {
    case pobr::BGR2HSV:
        cvt = std::make_shared<BGR2HSVConverter>();
        break;
    default:
        throw(std::out_of_range("Color conversion type not supported"));
    }
    return cvt;
}

cv::Vec3b BGR2HSVConverter::BGR2HSVPixelConverter::convert(const cv::Vec3b& bgr) {
    auto b = static_cast<double>(bgr[0]) / UCHAR_MAX;
    auto g = static_cast<double>(bgr[1]) / UCHAR_MAX;
    auto r = static_cast<double>(bgr[2]) / UCHAR_MAX;

    auto cmax = std::max(b, std::max(g, r));
    auto cmin = std::min(b, std::min(g, r));
    double diff = cmax - cmin;
    double h = 0., s = 0.;

    double v = cmax;

    if (diff > 10e-8) {
        if (cmax > 0.) {
            s = diff / cmax;
        }
        if (r >= cmax) {
            h = (g - b) / diff;
        }
        else if (g >= cmax) {
            h = 2. + (b - r) / diff;
        }
        else {
            h = 4. + (r - g) / diff;
        }

        h *= 60.;

        if ( h < 0.) {
            h += 360.;
        }
    }
    s *= UCHAR_MAX;
    v *= UCHAR_MAX;
    h /= 2.;
    return cv::Vec3b(h, s, v);
}

cv::Mat BGR2HSVConverter::convert(const cv::Mat& imageBGR) {
    cv::Mat imageHSV = imageBGR.clone();
    imageHSV.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int position[]) {
        px = pixelConverter_.convert(px);
    });
    return imageHSV;
}

cv::Vec3b BGR2HSVConverter::convert(const cv::Vec3b& vec) {
    return pixelConverter_.convert(vec);
}
