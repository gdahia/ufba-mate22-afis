#include "afis.hpp"

uchar afis::find_best_threshold(const cv::Mat & img) {
    /* finds the best threhsold for binarization using otsuo's method */
	uchar best = 0;
	double best_sb = 0;

    /* compute the probability of each gray level and total mean u */
    double p[256];
	for (int i = 0; i < 256; i++)
		p[i] = 0;
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
			p[img.at<uchar>(i, j)] += 1;
	double ut = 0;
	for (int i = 0; i < 256; i++) {
		p[i] /= img.rows*img.cols;
		ut += i*p[i];
	}


    /* compute best k */
    double w = 0, u = 0;
	for (int k = 0; k < 256; k++) {
		w += p[k];
		u += k*p[k];
		double sb = pow(ut*w - u, 2.0) / (w * (1 - w));
		if (sb > best_sb) {
			best_sb = sb;
			best = k;
		}
	}

	return best;
}
