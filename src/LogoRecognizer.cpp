#include "../include/LogoRecognizer.h"
#include "../include/processing/Utils.hpp"


LogoRecognizer::LogoRecognizer(std::string fileName) {
    sourceImage_ = cv::imread(fileName);
    auto converted = pobr::cvtColor(sourceImage_, pobr::BGR2HSV);

    // // red
    auto threshold = pobr::inRange(converted, cv::Vec3b(150, 110, 90), cv::Vec3b(179, 255, 255));
    auto threshold2 = pobr::inRange(converted, cv::Vec3b(0, 110, 90), cv::Vec3b(3, 255, 255));
    threshold = pobr::unite(threshold, threshold2);

    // // green
    // auto threshold = pobr::inRange(converted, cv::Vec3b(25, 70, 80), cv::Vec3b(87, 255, 255));
    

    // yellow
    // auto threshold = pobr::inRange(converted, cv::Vec3b(9, 60, 80), cv::Vec3b(22, 255, 255));

    auto segments = pobr::retrieveSegments(threshold, 250);
    std::cout << segments.size() << "\t";
    // auto segmentsMerged = pobr::mergeCloseSegments(segments, 0.4);
    auto segmentsMerged = segments;
    // std::cout << segmentsMerged.size() << "\n";

    // auto yellowSegments = colorPieceClassification(segmentsMerged, ColorPiece::YELLOW);

    // auto greenSegments = colorPieceClassification(segmentsMerged, ColorPiece::GREEN);

    auto redSegments = colorPieceClassification(segmentsMerged, ColorPiece::RED);
    for (auto& seg : redSegments) {
        seg.printCharacteristics();
        // if (seg.getArea() < 300) continue;
        // std::cout << seg.getArea() << "\t" << seg.getNM(1) << "\t" << seg.getNM(3) << "\t" << seg.getNM(7) << std::endl;
        // if (seg.getNM(1) < 0.2 || seg.getNM(1) > 0.35) continue;
        // if (seg.getNM(3) < 5 || seg.getNM(3) > 20) continue;
        // if (seg.getNM(7) < 0.009 || seg.getNM(7) > 0.02) continue;
        // std::cout << seg.getArea() << "\t" << seg.getNM(1) << "\t" << seg.getNM(3) << "\t" << seg.getNM(7) << std::endl;
        cv::Vec3b color(rand() % 255, rand() % 255, rand() % 255);
        seg.colorOnImage(threshold, color);
    }
    std::cout << std::endl;
    cv::imshow("Test" + fileName, threshold);
    // cv::waitKey(-1);
    // c.print();
}

std::vector<Segment> LogoRecognizer::colorPieceClassification(std::vector<Segment> segments, ColorPiece color) {
    std::vector<std::pair<double, double>> ranges;
    switch (color) {
    case ColorPiece::YELLOW:
        ranges = yellowRanges_;
        break;
    case ColorPiece::GREEN:
        ranges = greenRanges_;
        break;
    case ColorPiece::RED:
        ranges = redRanges_;
    }
    std::vector<Segment> pieces;
    for (auto& seg : segments) {
        std::vector<double> params = { seg.getW3(), seg.getNM(1), seg.getNM(7) };
        int validity = 0;
        for (int i = 0; i < ranges.size(); ++i) {
            if (pobr::utils::inRange<double>(params.at(i), ranges.at(i))) {
                ++validity;
            }
        }
        if (validity == ranges.size()) {
            pieces.push_back(seg);
        }
    }
    return pieces;
}