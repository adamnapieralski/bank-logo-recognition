#include <iostream>
#include <map>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

class Segment {
public:
    // Segment(cv::Vec3b colorValue, cv::Mat& mat);
    Segment();
    Segment(std::vector<cv::Point2i> points);

    void calculateParameters();

    int getPerimeter() const;
    int getArea() const;
    double getAngleDegrees() const;
    double getW3() const;
    double getNM(int n) const;
    cv::Mat getImgMarked() const;

    cv::Point2i getGeomCenter() const;
    cv::Point2i getMassCenter() const;

    void setFileName(std::string fileName);

    void colorOnImage(cv::Mat& image, const cv::Vec3b& color);
    bool isInNeighbourhood(const Segment& seg);

private:
    void retrievePoints(cv::Mat& mat);


    bool isEdgePoint(const cv::Point2i& p, const cv::Mat_<cv::Vec3b>& mat) const;
    void calculate_W3();
    double calculate_mpq(int p, int q);
    void calculate_massCenter();
    void calculate_geomCenter();
    void calculate_rectBorder();
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

    cv::Rect2i rectBorder_;
    double neighbourhoodRadiusMultiplicant_ = 2.;
    double neighbourhoodRadius_;

    std::vector<std::vector<double>> m_;
    std::vector<std::vector<double>> M_;
    std::map<int, double> NM_;
};