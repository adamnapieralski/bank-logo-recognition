#include "../include/LogoRecognizer.h"

LogoRecognizer::LogoRecognizer(std::string fileName) {
    sourceImage_ = cv::imread(fileName);
    auto converted = pobr::cvtColor(sourceImage_, pobr::BRG2HSV);
    // cv::imshow("Test", c.convert(sourceImage_));
    cv::imshow("Test", converted);
    cv::waitKey(-1);
    // c.print();
}