/**
 * @file Binarization.h
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */
#include <iostream>
#include <deque>

#include "../Segment.h"
#include "Consts.hpp"

namespace pobr {
    std::vector<Segment> retrieveSegments(const cv::Mat& image, const int minArea=300, const cv::Vec3b& val=consts::BINARY_PIXEL_WHITE);
    Segment retrieveSingleSegment(const cv::Mat& image, const cv::Point2i& origin);
    std::vector<Segment> mergeCloseSegments(std::vector<Segment> segments, double proximityRatio);
    std::deque<cv::Point2i> getNeighbors4p(const cv::Point2i& origin, const cv::Rect2i& size);
    std::deque<cv::Point2i> getNeighbors4p(const std::deque<cv::Point2i> origins, const cv::Rect2i& size);
}