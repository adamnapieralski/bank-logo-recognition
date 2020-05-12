#include <iostream>
#include <map>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

class Segment {
public:
    // Segment(cv::Vec3b colorValue, cv::Mat& mat);
    Segment();
    Segment(std::vector<cv::Point2i> points);

    int getPerimeter() const;
    int getArea() const;
    double getAngleDegrees() const;
    double getW3() const;
    double getNM(int n) const;
    cv::Mat getImgMarked() const;

    void setFileName(std::string fileName);

    std::string getTask1DataString() const;
    std::string getTask2DataString() const;

    void colorOnImage(cv::Mat& image, const cv::Vec3b& color);

private:
    void retrievePoints(cv::Mat& mat);
    void calculateParameters();

    bool isEdgePoint(const cv::Point2i& p, const cv::Mat_<cv::Vec3b>& mat) const;
    void calculate_W3();
    double calculate_mpq(int p, int q);
    void calculate_massCenter();
    void calculate_geomCenter();
    void calculate_angle();
    void calculate_m();
    void calculate_M();
    void calculate_NM();

    void markImg();

    cv::Vec3b colorValue_{0, 0, 0};
    
    std::string fileName_ = "";

    int perimeter_;
    int area_;
    double W3_;
    double angle_;

    cv::Mat imgMarked_;

    std::vector<cv::Point2i> points_;
    std::vector<cv::Point2i> edgePoints_;

    cv::Point2i massCenter_;
    cv::Point2i geomCenter_;

    std::vector<std::vector<double>> m_;
    std::vector<std::vector<double>> M_;
    std::map<int, double> NM_;
};