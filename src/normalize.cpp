#include <afis.hpp>

#ifndef M0
#define M0 100
#endif
#ifndef VAR0
#define VAR0 100
#endif

void afis::normalize(const cv::Mat & I, cv::Mat & G) {
    assert(I.channels() == 1 && I.type() == CV_8U);
    G = I.clone();
    cv::Scalar sm, sstd_dev;
    cv::meanStdDev(I, sm, sstd_dev);
    const double m = sm[0], var = sstd_dev[0]*sstd_dev[0];
    for (int i = 0; i < G.rows; i++) {
        for (int j = 0; j < G.cols; j++) {
            double tmp = sqrt((VAR0 * pow(I.at<uchar>(i, j) - m, 2.0)) / var);
            G.at<uchar>(i, j) = M0;
            if (I.at<uchar>(i, j) > m)
                G.at<uchar>(i, j) += tmp;
            else
                G.at<uchar>(i, j) -= tmp;
        }
    }
}
