#include "../include/Segment.h"
#include "../include/processing/Utils.hpp"
#include "../include/processing/Consts.hpp"

Segment::Segment() {}

Segment::Segment(std::vector<cv::Point2i> points) : points_(points) {}

Segment::Segment(std::vector<cv::Point2i> points, std::vector<cv::Point2i> edgePoints)
                : points_(points), edgePoints_(edgePoints) {}

int Segment::getPerimeter() const {
    return perimeter_;
}

int Segment::getArea() const {
    return area_;
}

double Segment::getAngleDegrees() const {
    return angle_ * 180 / M_PI;
}

double Segment::getW3() const {
    return W3_;
}

double Segment::getNM(int n) const {
    return NM_.at(n);
}


cv::Point2i Segment::getGeomCenter() const {
    return geomCenter_;
}

cv::Point2i Segment::getMassCenter() const {
    return massCenter_;
}

cv::Rect2i Segment::getRectBorder() const {
    return rectBorder_;
}

bool Segment::hasInNeighbourhood(const Segment& seg, double proximity, bool isOtherSeg) const {
    auto dist = pobr::utils::euclideanDistance<int>(seg.geomCenter_, geomCenter_);
    auto neighbourhoodRadius = proximity * pobr::utils::euclideanDistance<int>(rectBorder_.tl(), rectBorder_.br());
    if (dist < neighbourhoodRadius) {
        if (isOtherSeg) {
            return true;
        }
        else if (seg.hasInNeighbourhood(*this, proximity, true)) {
            return true;
        }
    }
    return false;
}

void Segment::printCharacteristics() const {
    printf("Area: %d\t Perim.: %d\tW3: %.1lf\tM1: %.6lf\tM3: %.1lf\tM7: %.6lf\n", area_, perimeter_, W3_, getNM(1), getNM(3), getNM(7));
}

void Segment::mergeIn(const Segment& seg) {
    points_.insert(points_.end(), seg.points_.begin(), seg.points_.end());
    edgePoints_.insert(edgePoints_.end(), seg.edgePoints_.begin(), seg.edgePoints_.end());
    calculateParameters();
}

Segment Segment::mergeOut(const Segment& seg) const {
    Segment merged;
    merged.points_.insert(merged.points_.end(), points_.begin(), points_.end());
    merged.points_.insert(merged.points_.end(), seg.points_.begin(), seg.points_.end());

    merged.edgePoints_.insert(merged.edgePoints_.end(), edgePoints_.begin(), edgePoints_.end());
    merged.edgePoints_.insert(merged.edgePoints_.end(), seg.edgePoints_.begin(), seg.edgePoints_.end());
    merged.calculateParameters();
    return merged;
}

void Segment::calculate_W3() {
    W3_ = static_cast<double>(perimeter_) / (2*sqrt(M_PI * area_)) - 1;
}

double Segment::calculate_mpq(int p, int q) {
    double mpq = 0;
    for (auto& point : points_) {
        mpq += pow(point.y + 1, p) * pow(point.x + 1, q);
    }
    return mpq;
}

void Segment::calculate_m() {
    m_.clear();
    for (int i = 0; i <= 3; ++i) {
        std::vector<double> mi;
        for (int j = 0; j <= 3; ++j) {
            mi.push_back(calculate_mpq(i, j));
        }
        m_.push_back(mi);
    }
}

void Segment::calculate_massCenter() {
    massCenter_.y = m_[1][0] / area_;
    massCenter_.x = m_[0][1] / area_;
}

void Segment::calculate_geomCenter() {
    geomCenter_ = cv::Point2d(rectBorder_.tl() + rectBorder_.br()) / 2.;
}

void Segment::calculate_rectBorder() {
    int xMin = points_.at(0).x;
    int xMax = xMin;
    int yMin = points_.at(0).y;
    int yMax = yMin;

    for (auto& point : points_) {
        if (point.x > xMax) xMax = point.x;
        if (point.x < xMin) xMin = point.x;
        if (point.y > yMax) yMax = point.y;
        if (point.y < yMin) yMin = point.y;
    }
    rectBorder_ = cv::Rect2i(cv::Point2i(xMin, yMin), cv::Point2i(xMax, yMax));
}

void Segment::calculate_angle() {
    cv::Point2d rel = massCenter_ - geomCenter_;
    angle_ = atan2(rel.y, rel.x);
}

void Segment::calculate_M() {
    M_ = {
        {
            static_cast<double>(area_),
            0.,
            m_[0][2] - m_[0][1]*m_[0][1] / area_,
            m_[0][3] - 3*m_[0][2]*(massCenter_.x+1) + 2*m_[0][1]*(massCenter_.x+1)*(massCenter_.x+1)
        },
        {
            0.,
            m_[1][1] - m_[1][0]*m_[0][1] / area_,
            m_[1][2] - 2*m_[1][1]*(massCenter_.x+1) - m_[0][2]*(massCenter_.y+1)
                + 2*m_[1][0]*(massCenter_.x+1)*(massCenter_.x+1),
            0.
        },
        {
            m_[2][0] - m_[1][0]*m_[1][0] / area_,
            m_[2][1] - 2*m_[1][1]*(massCenter_.y+1) - m_[2][0]*(massCenter_.x+1)
                + 2*m_[0][1]*(massCenter_.y+1)*(massCenter_.y+1),
            0.
        },
        {
            m_[3][0] - 3*m_[2][0]*(massCenter_.y+1) + 2*m_[1][0]*(massCenter_.y+1)*(massCenter_.y+1),
            0.,
            0.,
            0.
        }
    };
}

void Segment::calculate_NM() {
    NM_.insert(std::pair<int, double>(1, (M_[2][0] + M_[0][2]) / area_ / area_));
    NM_.insert(std::pair<int, double>(3,
            (pow(M_[3][0] - 3*M_[1][2], 2.) + pow(3*M_[2][1] - M_[0][3], 2.)) / pow(area_, 5.)));
    NM_.insert(std::pair<int, double>(7, (M_[2][0]*M_[0][2] - M_[1][1]*M_[1][1]) / pow(area_, 4.)));
}

void Segment::calculateParameters() {
    perimeter_ = edgePoints_.size();
    area_ = points_.size();
    calculate_W3();
    calculate_m();
    calculate_massCenter();
    calculate_rectBorder();
    calculate_geomCenter();
    calculate_angle();
    calculate_M();
    calculate_NM();
}

void Segment::colorOnImage(cv::Mat& image, const cv::Vec3b& color) const {
    cv::Mat_<cv::Vec3b> colorImg = image;
    for (auto& p : points_) {
        colorImg(p) = color;
    }
    for (auto& p : edgePoints_) {
        colorImg(p) = cv::Vec3b(0, 0, 255);
    }
    image = colorImg;
}

void Segment::markBorderOnImage(cv::Mat& image, const cv::Vec3b& color) const {
    cv::Mat_<cv::Vec3b> colorImg = image.clone();
    for (int i = rectBorder_.tl().y; i <= rectBorder_.br().y; ++i) {
        colorImg(i, rectBorder_.tl().x) = color;
        colorImg(i, rectBorder_.br().x) = color;
    }
    for (int i = rectBorder_.tl().x; i <= rectBorder_.br().x; ++i) {
        colorImg(rectBorder_.tl().y, i) = color;
        colorImg(rectBorder_.br().y, i) = color;
    }
    image = colorImg;
}
