#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

namespace afis {
    enum options {AFIS_SOBEL = 0, AFIS_MARR_HILDRETH = 1};

    uchar find_best_threshold(const cv::Mat &);

    void normalize(const cv::Mat &, cv::Mat &);

    void clahe(const cv::Mat &, cv::Mat &);

    void orientation_image(const cv::Mat &, cv::Mat &, const options option = AFIS_SOBEL);

    void draw_orientation(const cv::Mat &, const cv::Mat &, cv::Mat &);

    void wavelength_image(const cv::Mat &, const cv::Mat &, cv::Mat &);

    void gabor(const cv::Mat &, const cv::Mat &, const cv::Mat &, cv::Mat &);

    void enhance(const cv::Mat &, cv::Mat &);

    void thin(const cv::Mat &, cv::Mat &);

    void extract_minutiae(const cv::Mat &, std::vector<cv::Point> &);

    void save_minutiae_coordinates(const std::vector<cv::Point> &, const std::string &);

    void save_minutiae_markings(const std::vector<cv::Point> &, const cv::Mat &, const std::string &);

    double match(const std::vector<cv::Point> &, const std::vector<cv::Point> &);
}
