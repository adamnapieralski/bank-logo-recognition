#include "../include/LogoRecognizer.h"

LogoRecognizer::LogoRecognizer(std::string fileName) {
    auto image = cv::imread(fileName);
    sourceImage_ = image;
    BGR2HSVConverter c = BGR2HSVConverter();
    c.print();
}