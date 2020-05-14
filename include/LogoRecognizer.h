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

enum class ColorPiece {
    RED, GREEN, YELLOW
};

class LogoRecognizer {
public:
    LogoRecognizer(std::string fileName);

    std::vector<Segment> colorPieceClassification(std::vector<Segment> segments, ColorPiece color);

private:
    std::vector<std::pair<double, double>> yellowRanges_{
        {0.75, 1.75}, {0.18, 0.38}, {0.005, 0.022}
    };

    std::vector<std::pair<double, double>> greenRanges_{
        {0.45, 2.51}, {0.18, 0.34}, {0.007, 0.12}
    };

    std::vector<std::pair<double, double>> redRanges_{
        {0.66, 1.85}, {0.21, 0.28}, {0.011, 0.014}
    };
    



private:
    cv::Mat sourceImage_;
};

/**
 * NOTES ON COLORS :
 * HSV
 * - red : H 7.8/360    S 78,9/100  V 84,2 / 100
 * - yellow : H 31,5    S 80,7      V 92,1
 * - green : H 105,6    S 64,4      V 57,5
 * red: (0, 243 229), (3, 241, 205)
 * yellow: (18 ,255 , 244)
 * green: ( 66, 212, 160)
 * 
 * 
 */

#endif //_LOGORECOGNIZER_H_