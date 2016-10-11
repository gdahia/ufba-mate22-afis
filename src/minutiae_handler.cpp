#include <afis.hpp>
#include <fstream>
#include <utility>

void afis::save_minutiae_coordinates(const std::vector<cv::Point> & minutiae, const std::string & path) {
    std::ofstream output(path, std::ios::out);
    if (output.is_open()) {
        for (cv::Point p : minutiae)
            output << p.x << " " << p.y << std::endl;
    }
    else {
        std::cerr << "Error: " << path << " could not be opened" << std::endl;
        exit(1);
    }
}

void afis::read_minutiae_coordinates(std::vector<cv::Point> & minutiae, const std::string & path) {
    std::ifstream input(path, std::ios::in);
    if (input.is_open()) {
        int x, y;
        while (input >> x >> y)
            minutiae.emplace_back(x, y);
    }
    else {
        std::cerr << "Error: " << path << " could not be opened" << std::endl;
        exit(1);
    }
}

void afis::save_minutiae_markings(const std::vector<cv::Point> & minutiae, const cv::Mat & img, const std::string & path) {
    cv::Mat minutiae_markings;
    cv::cvtColor(img, minutiae_markings, CV_GRAY2BGR);

    for (cv::Point p : minutiae)
        cv::circle(minutiae_markings, p, 1, cv::Vec3b(0, 0, 255));

    cv::imwrite(path, minutiae_markings);
}
