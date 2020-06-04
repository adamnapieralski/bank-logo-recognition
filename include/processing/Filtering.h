/**
 * @file Filtering.h
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#ifndef _PROCESSING_FILTERING_H_
#define _PROCESSING_FILTERING_H_

#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "Consts.hpp"

namespace pobr {
    cv::Mat blurFilter(const cv::Mat& image);
    cv::Mat sharpenFilter(const cv::Mat& image);
}

#endif // _PROCESSING_FILTERING_H_