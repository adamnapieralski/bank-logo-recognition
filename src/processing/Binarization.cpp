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

cv::Mat pobr::unite(const cv::Mat& image1, const cv::Mat& image2) {
    if (image1.size != image2.size) {
        throw std::out_of_range("Images have different sizes. Cannot be united.");
    }
    cv::Mat_<cv::Vec3b> out = image1.clone();
    cv::Mat_<cv::Vec3b> img2 = image2.clone();
    for (int i = 0; i < out.rows; ++i) {
        for (int j = 0; j < out.cols; ++j) {
            if (out(i, j) != pobr::consts::BINARY_PIXEL_WHITE && img2(i, j) == pobr::consts::BINARY_PIXEL_WHITE) {
                out(i, j) = pobr::consts::BINARY_PIXEL_WHITE;
            }
        }
    }
    return out;
}

