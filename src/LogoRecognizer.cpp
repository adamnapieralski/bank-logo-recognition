#include "../include/LogoRecognizer.h"
#include "../include/processing/Utils.hpp"


LogoRecognizer::LogoRecognizer(std::string fileName) {
    sourceImage_ = cv::imread(fileName);

    auto colorSegs = findAllColorsSegments(sourceImage_);

    // auto converted = pobr::cvtColor(sourceImage_, pobr::BGR2HSV);
    // findAllColorsSegments(sourceImage_);
    // // // red
    // auto threshold = pobr::inRange(converted, cv::Vec3b(150, 110, 90), cv::Vec3b(179, 255, 255));
    // auto threshold2 = pobr::inRange(converted, cv::Vec3b(0, 110, 90), cv::Vec3b(3, 255, 255));
    // threshold = pobr::unite(threshold, threshold2);

    // // // green
    // // auto threshold = pobr::inRange(converted, cv::Vec3b(25, 70, 80), cv::Vec3b(87, 255, 255));
    

    // // yellow
    // // auto threshold = pobr::inRange(converted, cv::Vec3b(9, 60, 80), cv::Vec3b(22, 255, 255));

    // auto segments = pobr::retrieveSegments(threshold, 250);
    // std::cout << segments.size() << "\t";
    // // auto segmentsMerged = pobr::mergeCloseSegments(segments, 0.4);
    // auto segmentsMerged = segments;
    // // std::cout << segmentsMerged.size() << "\n";

    // // auto yellowSegments = colorPieceClassification(segmentsMerged, ColorPiece::YELLOW);

    // // auto greenSegments = colorPieceClassification(segmentsMerged, ColorPiece::GREEN);

    // auto redSegments = findColorSegments(segmentsMerged, ColorSegment::RED);
    // for (auto& seg : redSegments) {
    //     seg.printCharacteristics();
    //     cv::Vec3b color(rand() % 255, rand() % 255, rand() % 255);
    //     seg.colorOnImage(threshold, color);
    // }
    // std::cout << std::endl;
    // cv::imshow("Test" + fileName, threshold);

}

std::vector<Segment> findLogos(std::map<ColorSegment, std::vector<Segment>> colorSegments); 


std::vector<Segment> LogoRecognizer::findColorSegments(std::vector<Segment> segments, ColorSegment color) {
    std::vector<std::pair<double, double>> ranges;
    switch (color) {
    case ColorSegment::YELLOW:
        ranges = yellowRanges_;
        break;
    case ColorSegment::GREEN:
        ranges = greenRanges_;
        break;
    case ColorSegment::RED:
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

std::map<ColorSegment, std::vector<Segment>> LogoRecognizer::findAllColorsSegments(const cv::Mat& image) {
    std::map<ColorSegment, std::vector<Segment>> allColorsSegments;
    auto hsvImg = pobr::cvtColor(image, pobr::BGR2HSV);
    for (auto& color : colorsNames_) {
        cv::Mat threshold(image.size(), CV_8UC3, pobr::consts::BINARY_PIXEL_BLACK);
        for (auto& ranges : colorThresholdRanges_.at(color)) {
            threshold = pobr::unite(threshold, pobr::inRange(hsvImg, ranges.first, ranges.second));
        }
        auto segments = pobr::retrieveSegments(threshold, minSegmentSizes_.at(color));
        if (colorMergeProximities_.at(color) > pobr::consts::EPS) {
            segments = pobr::mergeCloseSegments(segments, colorMergeProximities_.at(color));
        }
        auto colorSegments = findColorSegments(segments, color);
        allColorsSegments.insert({ color, colorSegments });

        // for (auto& seg : colorSegments) {
        //     cv::Vec3b color(rand() % 255, rand() % 255, rand() % 255);
        //     seg.colorOnImage(threshold, color);
        //     seg.printCharacteristics();
        // }
        // std::cout << std::endl << std::endl;
        // if (color == ColorSegment::RED) {
        //     cv::imshow("Segments", threshold);
        //     cv::waitKey(-1);
        // }

    }
    return allColorsSegments;
}
