#include "../include/LogoRecognizer.h"
#include "../include/processing/Utils.hpp"

#include <fstream>

LogoRecognizer::LogoRecognizer(std::string fileName, bool isStageSaving) {
    filename_ = fileName;
    filenameBase_ = pobr::utils::getBaseFileName(fileName);
    filePathNoExtension_ = pobr::utils::getPathWithoutExtension(fileName);
    std::cout << filePathNoExtension_ << std::endl;
    sourceImage_ = cv::imread(fileName);
    isStagesSaving_ = isStagesSaving_;
}

void LogoRecognizer::performRecognition() {
    auto colorSegs = findAllColorsSegments(sourceImage_);
    foundLogos_ = findLogos(colorSegs);
    saveOutputData();
    cv::imshow("Output", sourceMarkedImg_);
    cv::waitKey(-1);
}

std::vector<Segment> LogoRecognizer::findLogos(std::map<ColorSegment, std::vector<Segment>> colorSegments) {
    std::vector<Segment> logos;
    
    for (auto& redSeg : colorSegments.at(ColorSegment::RED)) {
        for (auto& yellowSeg : colorSegments.at(ColorSegment::YELLOW)) {
            for (auto& greenSeg : colorSegments.at(ColorSegment::GREEN)) {
                if (redSeg.hasInNeighbourhood(yellowSeg, 1.5)) {
                    if (yellowSeg.hasInNeighbourhood(greenSeg, 1.5)) {
                        logos.push_back(redSeg.mergeOut(yellowSeg.mergeOut(greenSeg)));
                    }
                }
            }
        }
    }

    allColorsThresholdImg_ = cv::Mat(sourceImage_.size(), CV_8UC3, pobr::consts::BINARY_PIXEL_BLACK);
    sourceMarkedImg_ = sourceImage_.clone();
    for (auto& logo : logos) {
        logo.colorOnImage(allColorsThresholdImg_, pobr::utils::randomColor());
        logo.markBorderOnImage(sourceMarkedImg_, cv::Vec3b(0, 0, UCHAR_MAX));
    }

    return logos;
}


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
        break;
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
        colorThresholdImgs_.insert({color, threshold});

        auto segments = pobr::retrieveSegments(threshold, minSegmentSizes_.at(color));
        if (colorMergeProximities_.at(color) > pobr::consts::EPS) {
            segments = pobr::mergeCloseSegments(segments, colorMergeProximities_.at(color));
        }
        auto colorSegments = findColorSegments(segments, color);

        cv::Mat coloredImg(threshold.size(), CV_8UC3, pobr::consts::BINARY_PIXEL_BLACK);
        
        if (isStagesSaving_) {
            std::cout << "Found " + getColorSegmentName(color) + " color segments: " << std::endl;
        }

        for (auto& seg : colorSegments) {
            seg.colorOnImage(coloredImg, pobr::utils::randomColor());
            if (isStagesSaving_) {
                std::cout << "Center: " << seg.getGeomCenter() << "\t";
                seg.printCharacteristics();
            }
        }
        colorThresholdMarkedImgs_.insert({color, coloredImg});
        if (isStagesSaving_)    std::cout << std::endl;

        allColorsSegments.insert({ color, colorSegments });
    }
    return allColorsSegments;
}

std::string LogoRecognizer::getColorSegmentName(ColorSegment color) {
    switch (color) {
    case ColorSegment::RED:
        return "red";
        break;
    case ColorSegment::YELLOW:
        return "yellow";
        break;
    case ColorSegment::GREEN:
        return "green";
        break;
    }
}

void LogoRecognizer::saveOutputData() {
    if (isStagesSaving_) {
        for (auto& color : colorsNames_) {
            cv::imwrite(filePathNoExtension_ + "_thresh_" + getColorSegmentName(color) + ".png", colorThresholdImgs_.at(color));
            cv::imwrite(filePathNoExtension_ + "_thresh_colored_" + getColorSegmentName(color) + ".png", colorThresholdMarkedImgs_.at(color));
        }
        cv::imwrite(filePathNoExtension_ + "_thresh_all_colored.png", allColorsThresholdImg_);

        std::ofstream outfile;
        outfile.open(filePathNoExtension_ + "_out_log.txt");
        outfile << "Found logos data\nid\tx\ty\tw\th\n";
        for (size_t i = 0; i < foundLogos_.size(); ++i) {
            outfile << i << "\t" << foundLogos_.at(i).getRectBorder().x << "\t"
            << foundLogos_.at(i).getRectBorder().y << "\t"
            << foundLogos_.at(i).getRectBorder().width << "\t"
            << foundLogos_.at(i).getRectBorder().height << "\n";
        }
        outfile.close();
    }
    cv::imwrite(filePathNoExtension_ + "_marked_out.png", sourceMarkedImg_);
}