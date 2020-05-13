#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "../include/LogoRecognizer.h"


int main(int, char**)
{   
    // auto lr = LogoRecognizer("data/logo.jpg");
    for (int i = 1; i < 5; ++i) {
        auto lr = LogoRecognizer("data/default/img" + std::to_string(i) + "_logos.png");
    }
    cv::waitKey(-1);
    // auto lr = LogoRecognizer("data/logo_stripe2.jpg");
    return 0;
}
