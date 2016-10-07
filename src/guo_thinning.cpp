#include <afis.hpp>
#include <utility>
#include <algorithm>

bool subiteration1(cv::Mat & I, cv::Mat & S) {
    int lvisit[9] = {0, -1, -1, 0, 1, 1,  1,  0, -1};
    int rvisit[9] = {0,  0,  1, 1, 1, 0, -1, -1, -1};

    bool changed = false;

    for (int i = 1; i < I.rows - 1; i++) {
        for (int j = 1; j < I.cols - 1; j++) {
            if (I.at<uchar>(i, j)) {
                /* compute P and B */
                uchar p[9], b = 0;
                for (int k = 1; k < 9; k++) {
                    p[k] = (I.at<uchar>(i + lvisit[k], j + rvisit[k]) > 0);
                    b += p[k];
                }

                /* compute A */
                uchar a = 0;
                for (int k = 1; k < 8; k++)
                    a += p[k] == 0 && p[k + 1] > 0;
                a += p[8] == 0 && p[1] > 0;

                /* check if pixel is to be deleted */
                if (a == 1 && b >= 2 && b <= 6 && !(p[1] * p[3] * p[5]) && !(p[3] * p[5] * p[7])) {
                    S.at<uchar>(i, j) = 0;
                    changed = true;
                }
                else {
                    S.at<uchar>(i, j) = 255;
                }

            }
            else {
                S.at<uchar>(i, j) = 0;
            }
        }
    }

    std::swap(I, S);
    return changed;
}


bool subiteration2(cv::Mat & I, cv::Mat & S) {
    int lvisit[9] = {0, -1, -1, 0, 1, 1,  1,  0, -1};
    int rvisit[9] = {0,  0,  1, 1, 1, 0, -1, -1, -1};
    
    bool changed = false;

    for (int i = 1; i < I.rows - 1; i++) {
        for (int j = 1; j < I.cols - 1; j++) {
            if (I.at<uchar>(i, j)) {
                /* compute P and B */
                uchar p[9], b = 0;
                for (int k = 1; k < 9; k++) {
                    (p[k] = I.at<uchar>(i + lvisit[k], j + rvisit[k]) > 0);
                    b += p[k];
                }

                /* compute A */
                uchar a = 0;
                for (int k = 1; k < 8; k++)
                    a += p[k] == 0 && p[k + 1] > 0;
                a += p[8] == 0 && p[1] > 0;

                /* check if pixel is to be deleted */
                if (a == 1 && b >= 2 && b <= 6 && !(p[1] * p[3] * p[7]) && !(p[1] * p[5] * p[7])) {
                    S.at<uchar>(i, j) = 0;
                    changed = true;
                }
                else {
                    S.at<uchar>(i, j) = 255;
                }

            }
            else {
                S.at<uchar>(i, j) = 0;
            }

        }
    }

    std::swap(I, S);
    return changed;
}


void afis::thin(const cv::Mat & I, cv::Mat & S) {
    I.copyTo(S);
    cv::Mat M(S.size(), CV_8U);

    while (subiteration1(S, M) && subiteration2(S, M));
}
