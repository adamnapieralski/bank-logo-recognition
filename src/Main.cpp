#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "../include/LogoRecognizer.h"


int main(int, char**)
{   
    for (int i = 1; i < 2; ++i) {
        auto lr = LogoRecognizer("data/img" + std::to_string(i) + ".png", true);
        lr.performRecognition();
    }
    return 0;
}
