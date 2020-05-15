/**
 * @file LogoRecognizer.h
 * @brief
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#ifndef _LOGORECOGNIZER_H_
#define _LOGORECOGNIZER_H_

#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "processing/ColorConverters.h"
#include "processing/Binarization.h"
#include "processing/Segmentation.h"

enum class ColorSegment {
    RED, GREEN, YELLOW
};

class LogoRecognizer {
public:
    LogoRecognizer(std::string fileName, bool isStageSaving=false);

    void performRecognition();

    std::vector<Segment> findColorSegments(std::vector<Segment> segments, ColorSegment color);
    std::map<ColorSegment, std::vector<Segment>> findAllColorsSegments(const cv::Mat& image);
    std::vector<Segment> findLogos(std::map<ColorSegment, std::vector<Segment>> colorSegments); 

private:
    std::string getColorSegmentName(ColorSegment color);

    void saveOutputData();

    std::vector<ColorSegment> colorsNames_{
        ColorSegment::RED, ColorSegment::GREEN, ColorSegment::YELLOW
    };
    
    std::vector<std::pair<double, double>> yellowRanges_{
        {0.75, 1.75}, {0.18, 0.38}, {0.005, 0.022}
    };

    std::vector<std::pair<double, double>> greenRanges_{
        {0.45, 2.51}, {0.18, 0.34}, {0.007, 0.12}
    };

    std::vector<std::pair<double, double>> redRanges_{
        {0.66, 1.85}, {0.21, 0.28}, {0.011, 0.014}
    };

    std::map<ColorSegment, std::vector<std::pair<cv::Vec3b, cv::Vec3b>>> colorThresholdRanges_{
        { ColorSegment::RED, {
            { cv::Vec3b(150, 110, 90), cv::Vec3b(179, 255, 255) },
            { cv::Vec3b(0, 110, 90), cv::Vec3b(3, 255, 255) }
        }},
        { ColorSegment::GREEN, {
            { cv::Vec3b(25, 70, 80), cv::Vec3b(87, 255, 255) }
        }},
        { ColorSegment::YELLOW, {
            { cv::Vec3b(9, 60, 80), cv::Vec3b(22, 255, 255) }
        }}
    };
    
    std::map<ColorSegment, int> minSegmentSizes_{
        { ColorSegment::YELLOW, 200 },
        { ColorSegment::GREEN, 200 },
        { ColorSegment::RED, 250}
    };

    std::map<ColorSegment, double> colorMergeProximities_{
        { ColorSegment::YELLOW, 1.5 },
        { ColorSegment::GREEN, 0 },
        { ColorSegment::RED, 0}
    };

    cv::Mat sourceImage_;

    bool isStagesSaving_;

    std::string filename_;
    std::string filenameBase_;
    std::string filePathNoExtension_;

    std::map<ColorSegment, cv::Mat> colorThresholdImgs_;
    std::map<ColorSegment, cv::Mat> colorThresholdMarkedImgs_;
    cv::Mat allColorsThresholdImg_;
    cv::Mat sourceMarkedImg_;

    std::vector<Segment> foundLogos_;
};

#endif //_LOGORECOGNIZER_H_