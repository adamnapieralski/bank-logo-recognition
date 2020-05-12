/**
 * @file Segmentation.h
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#include "../../include/processing/Segmentation.h"

std::vector<Segment> pobr::retrieveSegments(const cv::Mat& image, const cv::Vec3b& val) {
    std::vector<Segment> segments;
    cv::Mat_<cv::Vec3b> img = image.clone();

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            cv::Point2i origin(j, i);
            if (img(origin) == val) {
                auto seg = retrieveSingleSegment(img, origin);
                seg.colorOnImage(img, consts::BINARY_PIXEL_BLACK);
                segments.push_back(seg);
            }
        }
    }
    return segments;
}

Segment pobr::retrieveSingleSegment(const cv::Mat& image, const cv::Point2i& origin) {
    std::vector<cv::Point2i> allPoints = {origin};
    std::deque<cv::Point2i> lastPoints = {origin};
    cv::Mat_<cv::Vec3b> img = image.clone();
    cv::Vec3b colorVal = img(origin);
    cv::Vec3b colorDiff = colorVal;

    while (colorDiff == colorVal) {
        colorDiff = cv::Vec3b(rand() % UCHAR_MAX);
    }
    img(origin) = colorDiff;

    while (!lastPoints.empty()){
        auto nbs = getNeighbors4p(lastPoints, cv::Rect2i(0, 0, img.cols, img.rows));
        lastPoints.clear();
        size_t size = nbs.size();
        for (size_t i = 0; i < size; ++i) {
            auto nb = nbs.front();
            if (img(nb) == colorVal) {
                img(nb) = colorDiff;
                lastPoints.push_back(nb);
            }
            nbs.pop_front();
        }
        for (auto& p : lastPoints) {
            allPoints.push_back(p);
        }
    }
    return Segment(allPoints);
}

std::deque<cv::Point2i> pobr::getNeighbors4p(const cv::Point2i& origin, const cv::Rect2i& size) {
    std::deque<cv::Point2i> nbs;
    std::vector<cv::Point2i> deltas = { cv::Point2i(1, 0), cv::Point2i(0, 1), cv::Point2i(-1, 0), cv::Point2i(0, -1) };
    for (auto& d : deltas) {
        auto nb = origin + d;
        if (size.contains(nb)) {
            nbs.push_back(nb);
        }
    }
    return nbs;
}

std::deque<cv::Point2i> pobr::getNeighbors4p(const std::deque<cv::Point2i> origins, const cv::Rect2i& size) {
    std::deque<cv::Point2i> nbs;
    for (auto& o : origins) {
        auto singleNbs = getNeighbors4p(o, size);
        for (auto& nb : singleNbs) {
            nbs.push_back(nb);
        }
    }
    return nbs;
}
