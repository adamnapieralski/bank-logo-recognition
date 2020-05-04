/**
 * @file Binarization.h
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#ifndef _PROCESSING_THRESHOLDING_H_
#define _PROCESSING_THRESHOLDING_H_

#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "Consts.hpp"

namespace pobr {
    bool isPixelInRange(const cv::Vec3b& pixel, const cv::Vec3b& lowerBound, const cv::Vec3b& upperBound);
    cv::Mat inRange(const cv::Mat& image, const cv::Vec3b& lowerBound, const cv::Vec3b& upperBound);
}






#endif // _PROCESSING_THRESHOLDING_H_