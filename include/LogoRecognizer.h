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

class LogoRecognizer {
public:
    LogoRecognizer(std::string fileName);



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