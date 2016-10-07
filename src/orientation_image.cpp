#include <afis.hpp>
#include <iostream>
#include <cmath>

#ifndef OR_WINDOW_SIZE
#define OR_WINDOW_SIZE 16
#endif

#ifndef PHI_WINDOW_SIZE
#define PHI_WINDOW_SIZE 5
#endif

void afis::orientation_image(const cv::Mat & I, cv::Mat & O, const options option) {
    cv::Mat dx, dy;

    /* compute dx and dy using the chosen operator */
    if (option == AFIS_SOBEL) {
        cv::Sobel(I, dx, CV_64F, 1, 0);
        cv::Sobel(I, dy, CV_64F, 0, 1);
    }
    else if (option == AFIS_MARR_HILDRETH) {
        std::cerr << "Error: Marr-Hildreth operator not yet supported" << std::endl;
        exit(1);
    }
    else {
        std::cerr << "Error: Operator for orientation image not supported. Options are: AFIS_SOBEL" << std::endl;
        exit(1);
    }

    const int hw = OR_WINDOW_SIZE / 2;

    /* compute integral images for fast computation */
    cv::Mat I_phi_x(I.rows, I.cols, CV_64F), I_phi_y(I.rows, I.cols, CV_64F);

    I_phi_x.at<double>(0, 0) = 2.0 * dx.at<double>(0, 0) * dy.at<double>(0, 0);
    I_phi_y.at<double>(0, 0) = pow(dx.at<double>(0, 0), 2.0) - pow(dy.at<double>(0, 0), 2.0);

    for (int j = 1; j < I.cols; j++) {
        I_phi_x.at<double>(0, j) = I_phi_x.at<double>(0, j - 1) + 2.0 * dx.at<double>(0, j) * dy.at<double>(0, j);
        I_phi_y.at<double>(0, j) = I_phi_y.at<double>(0, j - 1) + pow(dx.at<double>(0, j), 2.0) - pow(dy.at<double>(0, j), 2.0);
    }

    for (int i = 1; i < I.rows; i++) {
        double x_row_acum = 2.0 * dx.at<double>(i, 0) * dy.at<double>(i, 0);
        double y_row_acum = pow(dx.at<double>(0, 0), 2.0) - pow(dy.at<double>(0, 0), 2.0);
        I_phi_x.at<double>(i, 0) = x_row_acum + I_phi_x.at<double>(i - 1, 0);
        I_phi_y.at<double>(i, 0) = y_row_acum + I_phi_y.at<double>(i - 1, 0);
        for (int j = 1; j < I.cols; j++) {
            const double local_phi_x = 2.0 * dx.at<double>(i - 1, j - 1) * dy.at<double>(i - 1, j - 1);
            const double local_phi_y = pow(dx.at<double>(i - 1, j - 1), 2.0) - pow(dy.at<double>(i - 1, j - 1), 2.0);
            x_row_acum += local_phi_x;
            y_row_acum += local_phi_y;
            I_phi_x.at<double>(i, j) = I_phi_x.at<double>(i - 1, j) + x_row_acum;
            I_phi_y.at<double>(i, j) = I_phi_y.at<double>(i - 1, j) + y_row_acum;
        }
    }

    /* compute phi_x and phi_y matrices */
    cv::Mat phi_x(I.size(), CV_64F), phi_y(I.size(), CV_64F);
    for (int i = hw; i <= I.rows - hw; i++) {
        for (int j = hw; j <= I.cols - hw; j++) {
            phi_x.at<double>(i, j) = I_phi_x.at<double>(i + hw - 1, j + hw - 1) - I_phi_x.at<double>(i + hw - 1, j - hw) - I_phi_x.at<double>(i - hw, j + hw - 1) + I_phi_x.at<double>(i - hw, j - hw);
            phi_y.at<double>(i, j) = I_phi_y.at<double>(i + hw - 1, j + hw - 1) - I_phi_y.at<double>(i + hw - 1, j - hw) - I_phi_y.at<double>(i - hw, j + hw - 1) + I_phi_y.at<double>(i - hw, j - hw);
        }
    }

    cv::GaussianBlur(phi_x, phi_x, cv::Size(PHI_WINDOW_SIZE, PHI_WINDOW_SIZE), 0);
    cv::GaussianBlur(phi_y, phi_y, cv::Size(PHI_WINDOW_SIZE, PHI_WINDOW_SIZE), 0);

    /* compute orientation field */
    O.create(I.size(), CV_64F);
    for (int i = 0; i < O.rows; i++)
        for (int j = 0; j < O.cols; j++)
            O.at<double>(i, j) = atan2(phi_x.at<double>(i, j), phi_y.at<double>(i, j)) / 2.0;
}

void afis::draw_orientation(const cv::Mat & img, const cv::Mat & orient, cv::Mat & dst) {
    dst = img.clone();
    const int hw = OR_WINDOW_SIZE/2;
    for (int i = hw; i < img.rows; i += OR_WINDOW_SIZE) {
        for (int j = hw; j < img.cols; j += OR_WINDOW_SIZE) {
            double theta = orient.at<double>(i, j) - CV_PI * 0.5;
            cv::line(dst, cv::Point(j, i) - cv::Point(hw * cos(theta), hw * sin(theta)), cv::Point(hw * cos(theta), hw * sin(theta)) + cv::Point(j ,i), cv::Scalar(255));
        }
    }
}
