#include <afis.hpp>

void afis::enhance(const cv::Mat & I, cv::Mat & E) {
	/* normalize image */
	cv::Mat G;
	clahe(I, G);

	/* compute the orientation for each pixel */
	cv::Mat O;
	orientation_image(I, O);

	// cv::Mat map;
	// draw_orientation(G, O, map);
	// cv::imshow("orientation", map);

	cv::Mat L;
	wavelength_image(I, O, L);

	gabor(G, L, O, E);
}
