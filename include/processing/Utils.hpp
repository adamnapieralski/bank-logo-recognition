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

#include <iostream>
#include <string>

namespace pobr {
namespace utils {

    inline std::string getBaseFileName(std::string path) {
        std::string base = path.substr(path.find_last_of("/\\") + 1);
        base = base.substr(0, base.find_last_of('.'));
        return base;
    }

    inline std::string getPathWithoutExtension(std::string path) {
        return path.substr(0, path.find_last_of('.'));
    }

    inline cv::Vec3b randomColor() {
        return cv::Vec3b(rand() % UCHAR_MAX, rand() % UCHAR_MAX, rand() % UCHAR_MAX);
    }

    template<typename T>
    inline double euclideanDistance(cv::Point_<T> p1, cv::Point_<T> p2) {
        return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
    }

    template<typename T>
    inline bool inRange(T val, std::pair<T, T> range) {
        if (val >= range.first && val <= range.second) {
            return true;
        }
        return false;
    }
}
}

#endif // _PROCESSING_UTILS_HPP_