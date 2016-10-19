#include <afis.hpp>
#include <vector>
#include <string>

int main(int argc, char ** argv) {
    if (argc <= 2) {
        std::cerr << "Error: pass image path and output directory as parameters" << std::endl;
        return 1;
    }

    std::vector<cv::Point> minutiae_a, minutiae_b;

    afis::read_minutiae_coordinates(minutiae_a, "../digital/true/" + std::string(argv[1]) + ".txt");
    afis::read_minutiae_coordinates(minutiae_b, "../digital/true/" + std::string(argv[2]) + ".txt");

    std::cout << afis::match(minutiae_a, minutiae_b) << std::endl;
}
