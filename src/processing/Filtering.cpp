/**
 * @file Filtering.cpp
 * 
 * @author Adam Napieralski
 * @date 05.2020
 */

#include "../../include/processing/Filtering.h"

cv::Mat pobr::blurFilter(const cv::Mat& image) {
    cv::Mat_<cv::Vec3b> res = image.clone();
    cv::Mat_<cv::Vec3b> org = image.clone();

    for (int i = 1; i < image.rows - 1; ++i) {
        for (int j = 1; j < image.cols - 1; ++j) {
            // cv::Vec3b px = res.at(i, j);
            for (int k = 0; k < 3; ++k) {
                double conv = 0;
                for (int m = 0; m < 3; ++m) {
                    for (int n = 0; n < 3; ++n) {
                        conv += org[i + m - 1][j + n - 1][k] * consts::lowpassFilterMatrix[m][n];
                    }
                }
                if (conv > 255) conv = 255;
                else if (conv < 0) conv = 0;
                res[i][j][k] = conv;
            }
        }
    }
    cv::Mat out = res;  
    return out;
}
cv::Mat pobr::sharpenFilter(const cv::Mat& image) {
    cv::Mat out = image.clone();
    cv::Mat_<cv::Vec3b> res = image.clone();
    cv::Mat_<cv::Vec3b> org = image.clone();
    
    for (int i = 0; i < org.rows; ++i) {
        for (int j = 0; j < org.cols; ++j) {
            auto px = org(i, j);
            org(i, j)[0] = (px[0] + px[1] + px[2]) / 3;
        }
    }

    for (int i = 1; i < org.rows - 1; ++i) {
        for (int j = 1; j < org.cols - 1; ++j) {             
            int conv = 0;
            for (int m = 0; m < 3; ++m) {
                for (int n = 0; n < 3; ++n) {
                    conv += org(i + m - 1, j + n - 1)[0] * consts::highpassFilterMatrix[m][n];
                }
            }
            if (conv > 255) conv = 255;
            else if (conv < 0) conv = 0;
            for (int k = 0; k < 3; ++k) {
                res(i, j)[k] = conv;
            }
        }
    }
    out = res;
    return out;
}
