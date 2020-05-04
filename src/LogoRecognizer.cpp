#include "../include/LogoRecognizer.h"

LogoRecognizer::LogoRecognizer(std::string fileName) {
    sourceImage_ = cv::imread(fileName);
    auto converted = pobr::cvtColor(sourceImage_, pobr::BRG2HSV);
    auto threshold = pobr::inRange(converted, cv::Vec3b(0, 100, 100), cv::Vec3b(1, 255, 255));
    // cv::imshow("Test", c.convert(sourceImage_));
    cv::imshow("Test", threshold);
    cv::waitKey(-1);
    // c.print();
}