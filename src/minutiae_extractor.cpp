#include <afis.hpp>
#include <vector>

void afis::extract_minutiae(const cv::Mat & S, std::vector<cv::Point> & minutiae) {
    int lvisit[9] = {0, -1, -1, 0, 1, 1,  1,  0, -1};
    int rvisit[9] = {0,  0,  1, 1, 1, 0, -1, -1, -1};

    for (int i = 2; i < S.rows - 2; i++) {
        for (int j = 2; j < S.cols - 2; j++) {
            if (!S.at<uchar>(i, j)) {
                /* compute ridge-neighbors pattern */
                uchar b = 0, p[9];
                for (int k = 1; k < 9; k++) {
                    p[k] = !(S.at<uchar>(i + lvisit[k], j + rvisit[k]));
                    b += p[k];
                }

                /* remove pseudo-bifurcations */
                for (int k = 2; k < 9; k++) {
                    if (p[k] && p[k - 1])
                        b--;
                }
                if (p[8] && p[1]) b--;

                if (b == 1 || b == 3)
                    minutiae.emplace_back(j, i);
            }
        }
    }
}
