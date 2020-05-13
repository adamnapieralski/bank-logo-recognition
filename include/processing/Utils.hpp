/**
 * @file Utils.h
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#ifndef _PROCESSING_UTILS_HPP_
#define _PROCESSING_UTILS_HPP_

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

namespace pobr {
namespace utils {
    template<typename T>
    double euclideanDistance(cv::Point_<T> p1, cv::Point_<T> p2) {
        return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
    }
}
}

#endif // _PROCESSING_UTILS_HPP_