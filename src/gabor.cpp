#include <afis.hpp>

#ifndef GABOR_FILTER_SIZE
#define GABOR_FILTER_SIZE 30
#endif

#ifndef SIGMA
#define SIGMA 8.0
#endif

void afis::gabor(const cv::Mat & G, const cv::Mat & L, const cv::Mat & O, cv::Mat & E) {
    E = cv::Mat::zeros(G.rows - GABOR_FILTER_SIZE, G.cols - GABOR_FILTER_SIZE, CV_64F);
    cv::Mat g;
    G.convertTo(g, CV_64F);
	cv::Size kernel_size(GABOR_FILTER_SIZE, GABOR_FILTER_SIZE);
    const int hw = GABOR_FILTER_SIZE / 2;
    for (int i = hw; i < G.rows - hw; i++) {
        for (int j = hw; j < G.cols - hw; j++) {
            cv::Mat kernel = cv::getGaborKernel(kernel_size, SIGMA, O.at<double>(i, j), L.at<double>(i, j), 1.0, 0, CV_64F);
			double pixel = 0;
			for (int v = -hw; v <= hw; v++)
				for (int u = -hw; u <= hw; u++)
					pixel += kernel.at<double>(u + hw, v + hw) * g.at<double>(i + u, j + v);
			E.at<double>(i - hw, j - hw) = pixel;
        }
    }
	cv::Mat I(E.size(), CV_8U);
	I = E > 1.0;
	std::swap(E, I);
}
