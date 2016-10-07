#include <afis.hpp>

void afis::clahe(const cv::Mat & I, cv::Mat & G) {
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(4);
    clahe->setTilesGridSize(cv::Size(8, 8));
    clahe->apply(I, G);
}
