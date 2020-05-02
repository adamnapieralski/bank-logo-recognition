#include "../include/LogoRecognizer.h"

LogoRecognizer::LogoRecognizer(std::string fileName) {
    sourceImage_ = cv::imread(fileName);
    BGR2HSVConverter c = BGR2HSVConverter();
    cv::imshow("Test", c.convert(sourceImage_));
    // cv::imshow("Test", sourceImage_);
    cv::waitKey(-1);
    // c.print();
}