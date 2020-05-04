/**
 * @file Consts.h
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#ifndef _PROCESSING_CONSTS_HPP_
#define _PROCESSING_CONSTS_HPP_

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

namespace pobr::consts {
    const uint8_t WHITE = 255;
    const uint8_t BLACK = 0;

    const cv::Vec3b BINARY_PIXEL_WHITE = cv::Vec3b(WHITE, WHITE, WHITE);
    const cv::Vec3b BINARY_PIXEL_BLACK = cv::Vec3b(BLACK, BLACK, BLACK);
}

#endif //_PROCESSING_CONSTS_HPP_