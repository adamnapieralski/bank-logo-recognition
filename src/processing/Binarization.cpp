/**
 * @file Binarization.h
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#include "../../include/processing/Binarization.h"

bool pobr::isPixelInRange(const cv::Vec3b& pixel, const cv::Vec3b& lowerBound, const cv::Vec3b& upperBound) {
    for (int i = 0; i < pixel.channels; ++i) {
        if (pixel[i] < lowerBound[i] || pixel[i] > upperBound[i]) {
            return false;
        }
    }
    return true;
}

cv::Mat pobr::inRange(const cv::Mat& image, const cv::Vec3b& lowerBound, const cv::Vec3b& upperBound) {
    auto res = image.clone();
    res.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int position[]) {
        if (pobr::isPixelInRange(px, lowerBound, upperBound)) {
            px = consts::BINARY_PIXEL_WHITE;
        }
        else {
            px = consts::BINARY_PIXEL_BLACK;
        }
    });
    return res;
}
