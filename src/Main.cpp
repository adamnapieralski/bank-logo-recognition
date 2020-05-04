#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "../include/LogoRecognizer.h"


int main(int, char**)
{   
    auto lr = LogoRecognizer("data/img5.png");
    return 0;
}
