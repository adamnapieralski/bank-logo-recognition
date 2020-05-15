#include <iostream>
#include <map>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

class Segment {
public:
    Segment();
    Segment(std::vector<cv::Point2i> points);
    Segment(std::vector<cv::Point2i> points, std::vector<cv::Point2i> edgePoints);

    void calculateParameters();

    int getPerimeter() const;
    int getArea() const;
    double getAngleDegrees() const;
    double getW3() const;
    double getNM(int n) const;
    cv::Rect2i getRectBorder() const;

    cv::Point2i getGeomCenter() const;
    cv::Point2i getMassCenter() const;

    void colorOnImage(cv::Mat& image, const cv::Vec3b& color) const;
    void markBorderOnImage(cv::Mat& image, const cv::Vec3b& color) const;

    bool hasInNeighbourhood(const Segment& seg, double proximity, bool isOtherSeg=false) const;

    void printCharacteristics() const;

    void mergeIn(const Segment& seg);
    Segment mergeOut(const Segment& seg) const;

private:
    bool isEdgePoint(const cv::Point2i& p, const cv::Mat_<cv::Vec3b>& mat) const;

    void calculate_W3();
    double calculate_mpq(int p, int q);
    void calculate_massCenter();
    void calculate_geomCenter();
    void calculate_rectBorder();
    void calculate_perimeter();
    void calculate_angle();
    void calculate_m();
    void calculate_M();
    void calculate_NM();
    
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

    std::vector<std::vector<double>> m_;
    std::vector<std::vector<double>> M_;
    std::map<int, double> NM_;
};