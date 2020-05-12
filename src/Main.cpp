#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "../include/LogoRecognizer.h"


int main(int, char**)
{   
    auto lr = LogoRecognizer("data/img5.png");
    // auto lr = LogoRecognizer("data/logo_stripe2.jpg");
    return 0;
}
