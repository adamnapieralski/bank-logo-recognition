#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "../include/LogoRecognizer.h"


int main(int argc, char** argv)
{   
    if (argc < 2 || argc > 3) {
        std::cout << "Incorrect number of arguments." << std::endl;
        return -1;
    }
    std::string fileName = argv[1];
    bool savingStagesOpt = false;
    if (argc == 3) {
        std::string opt = argv[2];
        if (opt == "-s") {
            savingStagesOpt = true;
        }
        else {
            std::cout << "Incorrect option." << std::endl;
            return -1;
        }
    }
    try {
        auto lr = LogoRecognizer(fileName, savingStagesOpt);
        lr.performRecognition();
    }
    catch (...){
        std::cout << "Recognition failed." << std::endl;
    }
    return 0;
}
