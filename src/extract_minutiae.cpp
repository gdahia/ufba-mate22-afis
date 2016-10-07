#include <afis.hpp>
#include <vector>
#include <string>

int main(int argc, char ** argv) {
    if (argc <= 2) {
        std::cerr << "Error: pass image path and output directory as parameters" << std::endl;
        return 1;
    }

    cv::Mat img = cv::imread(argv[1], 0);

    cv::Mat enhanced;
    afis::enhance(img, enhanced);

    cv::Mat skeleton;
    enhanced = enhanced < 255;
    afis::thin(enhanced, skeleton);
    enhanced = enhanced < 255;
    skeleton = skeleton < 255;

    std::vector<cv::Point> minutiae;
    afis::extract_minutiae(skeleton, minutiae);

    afis::save_minutiae_coordinates(minutiae, std::string(argv[2]) + "coordinates.txt");
    afis::save_minutiae_markings(minutiae, skeleton, std::string(argv[2]) + "image.pgm");
}
