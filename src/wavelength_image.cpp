#include <afis.hpp>
#include <cmath>

#ifndef L_WINDOW_SIZE
#define L_WINDOW_SIZE 32
#endif

#ifndef WL_WINDOW_SIZE
#define WL_WINDOW_SIZE 16
#endif

#ifndef OMEGA_WINDOW_SIZE
#define OMEGA_WINDOW_SIZE 7
#endif

double wavelength(double sign[L_WINDOW_SIZE]) {
    static const double sqrt_2 = sqrt(2);

    /* describe curve */
    double max = sign[0], mean = 0;
    for (int i = 0; i < L_WINDOW_SIZE; i++) {
        mean += sign[i];
        max = std::max(sign[i], max);
    }
    mean /= L_WINDOW_SIZE;
    const double amplitude = (max - mean) / sqrt_2;

    /* find peak regions */
    bool peak_region[L_WINDOW_SIZE];
    for (int i = 0; i < L_WINDOW_SIZE; i++) {
        if (sign[i] > mean + amplitude)
            peak_region[i] = true;
        else
            peak_region[i] = false;
    }

    /* compute peak region center */
    int l = 0, r = 0, n_peaks = 0;
    bool l_ready = false, r_ready = false;
    bool is_peak[L_WINDOW_SIZE];
    for (int i = 0; i < L_WINDOW_SIZE; i++) {
        is_peak[i] = false;
        if (peak_region[i]) {
            if (i == 0 || !peak_region[i - 1]) {
                l = i;
                l_ready = true;
            }

            if (i == L_WINDOW_SIZE - 1 || !peak_region[i + 1]) {
                r = i;
                r_ready = true;
            }
            if (l_ready && r_ready) {
                is_peak[(l + r) >> 1] = true;
                l_ready = r_ready = false;
                n_peaks++;
            }
        }
    }

    if (n_peaks <= 1)
        return 15;

    /* compute average number of pixels betwen two consecutive peaks */
    double tau = 0;
    int last_peak = 0;
    for (;last_peak < L_WINDOW_SIZE && !is_peak[last_peak]; last_peak++);
    for (int k = last_peak + 1; k < L_WINDOW_SIZE; k++) {
        if (is_peak[k]) {
            tau += k - last_peak + 1;
            last_peak = k;
            k++;
        }
    }

    return tau / (n_peaks - 1);
}

void afis::wavelength_image(const cv::Mat & G, const cv::Mat & O, cv::Mat & L) {
    const double hw = WL_WINDOW_SIZE / 2.0;
    const double lhw = L_WINDOW_SIZE / 2.0;

    cv::Mat omega(G.size(), CV_64F);
    for (int i = 0; i < G.rows; i++) {
        for (int j = 0; j < G.cols; j++) {

            const double theta = O.at<double>(i, j);
			const double cos_theta = cos(theta);
			const double sin_theta = sin(theta);

            /* compute the x-signature */
            double x[L_WINDOW_SIZE];
            for (int k = 0; k < L_WINDOW_SIZE; k++) {
                x[k] = 0;
                for (int d = 0; d < WL_WINDOW_SIZE; d++) {
                    const unsigned int u = i + (d - hw) * cos_theta + (k - lhw) * sin_theta;
                    const unsigned int v = j - (d - hw) * sin_theta + (k - lhw) * cos_theta;
                    x[k] += G.at<uchar>(u, v);
                }
                x[k] /= WL_WINDOW_SIZE;
            }

            const double l = wavelength(x);
            omega.at<double>(i, j) = l;
        }
    }

    L.create(omega.size(), CV_64F);
    const int half_window = OMEGA_WINDOW_SIZE / 2;
    for (int i = half_window; i <= omega.rows - half_window; i++)
        for (int j = half_window; j <= omega.cols - half_window; j++) {
            double wavelength = 0;
            int q = 0;
            for (int u = -half_window; u < half_window; u++)
                for (int v = -half_window; v < half_window; v++)
                    if (omega.at<double>(i + u, j + v) != 0) {
                        q++;
                        wavelength += omega.at<double>(i + u, j + v);
                    }
            L.at<double>(i, j) = wavelength / q;
        }

    // /* compute omega iteratively */
    // cv::Mat pre_omega(G.size(), CV_64F), u_blurred_pre_omega(G.size(), CV_64F), d_blurred_pre_omega(G.size(), CV_64F);
    // while (still_pre) {
    //     std::cout << "got here" << std::endl;
    //     swap(omega, pre_omega);
    //
    //     /* compute u_blurred_pre_omega and d_blurred_pre_omega */
    //     for (int i = 0; i < pre_omega.rows; i++) {
    //         for (int j = 0; j < pre_omega.cols; j++) {
    //             u_blurred_pre_omega.at<double>(i, j) = (pre_omega.at<double>(i, j) > 0 ? pre_omega.at<double>(i, j) : 0);
    //             d_blurred_pre_omega.at<double>(i, j) = (pre_omega.at<double>(i, j) + 1 > 0 ? 1 : 0);
    //         }
    //     }
    //     cv::GaussianBlur(u_blurred_pre_omega, u_blurred_pre_omega, cv::Size(OMEGA_WINDOW_SIZE, OMEGA_WINDOW_SIZE), 3);
    //     cv::GaussianBlur(d_blurred_pre_omega, d_blurred_pre_omega, cv::Size(OMEGA_WINDOW_SIZE, OMEGA_WINDOW_SIZE), 3);
    //
    //     still_pre = false;
    //     for (int i = 0; i < pre_omega.rows; i++) {
    //         for (int j = 0; j < pre_omega.cols; j++) {
    //             if (pre_omega.at<double>(i, j) != 0)
    //                 omega.at<double>(i, j) = pre_omega.at<double>(i, j);
    //             else {
    //                 omega.at<double>(i, j) = d_blurred_pre_omega.at<double>(i, j)/u_blurred_pre_omega.at<double>(i, j);
    //                 still_pre = true;
    //             }
    //         }
    //     }
    //
    // }
    //
    // cv::GaussianBlur(omega, L, cv::Size(OMEGA_WINDOW_SIZE, OMEGA_WINDOW_SIZE), 0);
}
