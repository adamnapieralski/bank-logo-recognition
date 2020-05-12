#include "../include/Segment.h"

// Segment::Segment(cv::Vec3b colorValue, cv::Mat& mat) {
//     colorValue_ = colorValue;
//     retrievePoints(mat);
//     calculateParameters();
//     imgMarked_ = cv::Mat(mat.rows, mat.cols, CV_8UC3);
//     markImg();
// }

Segment::Segment() {}

Segment::Segment(std::vector<cv::Point2i> points) {
    points_ = points;
}

bool Segment::isEdgePoint(const cv::Point2i& p, const cv::Mat_<cv::Vec3b>& mat) const {
    bool diff = false;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            cv::Point2i nb = p + cv::Point2i(j, i);
            cv::Rect2i matRect = cv::Rect2i(0, 0, mat.cols, mat.rows);
            if (nb == p || !nb.inside(matRect)) continue;
            if (mat(p) == colorValue_ && mat(nb) != colorValue_) {
                diff = true;
                return diff;
            }
        }
    }
    return diff;
}

void Segment::retrievePoints(cv::Mat& mat) {
    cv::Mat_<cv::Vec3b> mat_ = mat;
    for (int r = 0; r < mat_.rows; ++r) {
        for (int c = 0; c < mat_.cols; ++c) {
            auto p = cv::Point2i(c, r);
            if (mat_(p) == colorValue_) {
                points_.push_back(p);
                if (isEdgePoint(p, mat_)) {
                    edgePoints_.push_back(p);
                }
            }
        }
    }
}

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

cv::Mat Segment::getImgMarked() const {
    return imgMarked_;
}

void Segment::setFileName(std::string fileName) {
    fileName_ = fileName;
}


std::string Segment::getTask1DataString() const {
    std::string data = "Plik " + fileName_
                        + ", S=" + std::to_string(getArea())
                        + ", L=" + std::to_string(getPerimeter())
                        + ", W3=" + std::to_string(getW3())
                        + ", M1=" + std::to_string(getNM(1))
                        + ", M7=" + std::to_string(getNM(7));
    return data;
}
std::string Segment::getTask2DataString() const {
    std::string data = "Strzalka R=" + std::to_string(colorValue_[2])
                        + ", nachylenie " + std::to_string(getAngleDegrees())
                        + ", S=" + std::to_string(getArea())
                        + ", L=" + std::to_string(getPerimeter())
                        + ", W3=" + std::to_string(getW3())
                        + ", M1=" + std::to_string(getNM(1))
                        + ", M7=" + std::to_string(getNM(7));
    return data;
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
    geomCenter_ = cv::Point2d((xMax + xMin) / 2., (yMax + yMin) / 2.);
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
    calculate_geomCenter();
    calculate_angle();
    calculate_M();
    calculate_NM();
}

void Segment::markImg() {
    cv::Mat_<cv::Vec3b> marked = imgMarked_;
    for (auto& p : points_) {
        marked(p) = cv::Vec3b(255, 0, 0);
    }
    for (auto& e : edgePoints_) {
        marked(e) = cv::Vec3b(0, 0, 255);
    }
    marked(massCenter_) = cv::Vec3b(0, 255, 255);
    marked(geomCenter_) = cv::Vec3b(255, 255, 0);
    imgMarked_ = marked;
}

void Segment::colorOnImage(cv::Mat& image, const cv::Vec3b& color) {
    cv::Mat_<cv::Vec3b> colorImg = image;
    for (auto& p : points_) {
        colorImg(p) = color;
    }
    image = colorImg;
}