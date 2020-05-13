#include "../include/LogoRecognizer.h"
#include "../include/processing/Utils.hpp"


LogoRecognizer::LogoRecognizer(std::string fileName) {
    sourceImage_ = cv::imread(fileName);
    auto converted = pobr::cvtColor(sourceImage_, pobr::BGR2HSV);

    // // red
    // auto threshold = pobr::inRange(converted, cv::Vec3b(150, 110, 110), cv::Vec3b(179, 255, 255));
    // auto threshold2 = pobr::inRange(converted, cv::Vec3b(0, 110, 110), cv::Vec3b(1, 255, 255));
    // threshold = pobr::unite(threshold, threshold2);

    // // green
    // auto threshold = pobr::inRange(converted, cv::Vec3b(25, 50, 80), cv::Vec3b(87, 255, 255));
    

    // yello
    auto threshold = pobr::inRange(converted, cv::Vec3b(9, 60, 80), cv::Vec3b(22, 255, 255));

    auto segments = pobr::retrieveSegments(threshold, 200);
    std::cout << segments.size() << std::endl;
    auto segmentsMerged = pobr::mergeCloseSegments(segments, 1.5);
    for (auto& seg : segmentsMerged) {
        // if (seg.getArea() < 300) continue;
        // std::cout << seg.getArea() << "\t" << seg.getNM(1) << "\t" << seg.getNM(3) << "\t" << seg.getNM(7) << std::endl;
        // if (seg.getNM(1) < 0.2 || seg.getNM(1) > 0.35) continue;
        // if (seg.getNM(3) < 5 || seg.getNM(3) > 20) continue;
        // if (seg.getNM(7) < 0.009 || seg.getNM(7) > 0.02) continue;
        // std::cout << seg.getArea() << "\t" << seg.getNM(1) << "\t" << seg.getNM(3) << "\t" << seg.getNM(7) << std::endl;
        cv::Vec3b color(rand() % 255, rand() % 255, rand() % 255);
        seg.colorOnImage(threshold, color);
    }
    cv::imshow("Test" + fileName, threshold);
    // cv::waitKey(-1);
    // c.print();
}