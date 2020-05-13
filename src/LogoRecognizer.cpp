#include "../include/LogoRecognizer.h"

LogoRecognizer::LogoRecognizer(std::string fileName) {
    auto vec = cv::Vec3b(30, 20, 200);
    std::cout << pobr::cvtColor(vec, pobr::BGR2HSV);

    sourceImage_ = cv::imread(fileName);
    auto converted = pobr::cvtColor(sourceImage_, pobr::BGR2HSV);
    auto threshold = pobr::inRange(converted, cv::Vec3b(177, 120, 120), cv::Vec3b(179, 255, 255));
    // cv::imshow("Test", c.convert(sourceImage_));



    auto segments = pobr::retrieveSegments(threshold);
    for (auto& seg : segments) {
        seg.calculateParameters();
        if (seg.getArea() < 300) continue;
        // std::cout << seg.getArea() << "\t" << seg.getNM(1) << "\t" << seg.getNM(3) << "\t" << seg.getNM(7) << std::endl;
        if (seg.getNM(1) < 0.2 || seg.getNM(1) > 0.35) continue;
        // if (seg.getNM(3) < 5 || seg.getNM(3) > 20) continue;
        if (seg.getNM(7) < 0.009 || seg.getNM(7) > 0.02) continue;
        std::cout << seg.getArea() << "\t" << seg.getNM(1) << "\t" << seg.getNM(3) << "\t" << seg.getNM(7) << std::endl;
        cv::Vec3b color(0, 0, 255);
        seg.colorOnImage(threshold, color);
    }
    cv::imshow("Test" + fileName, threshold);
    // cv::waitKey(-1);
    // c.print();
}