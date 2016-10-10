#include <afis.hpp>
#include <limits>
#include <algorithm>

struct Rotator {
    const double rot_cos, rot_sin;

    Rotator(double argcos, double argsin) : rot_cos(argcos), rot_sin(argsin) {}

    cv::Point2d operator()(const cv::Point & p) {
        return cv::Point2d(p.x * rot_cos - p.y * rot_sin, p.x * rot_sin + p.y * rot_cos);
    }
};

double min_cost_max_flow(const std::vector<cv::Point> & minutiae_a, const std::vector<cv::Point2d> & minutiae_b) {
    const int len_a = minutiae_a.size();
    const int len_b = minutiae_b.size();

    /* prepare adjacency list */
    std::vector<std::pair<int, double>> adjs[len_a + len_b + 2];

    /* source is connected to every node in minutiae set A with cost 0 */
    for (int i = 1; i <= len_a; i++)
        adj[0].emplace_back(i, 0);

    /* every node in minutiae set B  is connected to sink with cost 0 */
    for (int i = 1; i <= len_b; i++)
        adj[i + len_a].emplace_back(len_a + len_b + 1, 0);

    /* connect every minutia in A to every minutia in B with their distance as cost */
    for (int i = 0; i < len_a; i++)
        for (int j = 0; j < len_b; j++)
            adj[i + 1].emplace_back(len_a + j + 1, cv::norm(minutiae_a[i] - minutiae_b[j]));

    
}

/* computes the most similar possible match assuming idx_b is the minutia match in set B of minutia 0 in set A */
double compute_similarity(const int idx_b, const std::vector<cv::Point> & minutiae_a, const std::vector<cv::Point> & minutiae_b) {
    double similarity = std::numeric_limits<double>::max();
    const cv::Point2d ref = minutiae_a[1] - minutiae_a[0];

    for (int i = idx_b + 1, len_b = minutiae_b.size(); i < len_b; i++) {
        const cv::Point2d vec = minutiae_b[1] - minutiae_b[0];
        const double norm_prod = cv::norm(ref) * cv::norm(vec);
        const double rot_cos = ref.ddot(vec) / norm_prod;
        const double rot_sin = ref.cross(vec) / norm_prod;

        std::vector<cv::Point2d> rotated_minutiae_b(len_b);
        std::transform(minutiae_b.begin(), minutiae_b.end(), rotated_minutiae_b.begin(), Rotator(rot_cos, rot_sin));

        similarity = std::min(similarity, min_cost_max_flow(minutiae_a, rotated_minutiae_b));
    }

    return similarity;
}

/* finds the closest match between two minutiae coordinates */
double afis::match(std::vector<cv::Point> & minutiae_a, std::vector<cv::Point> & minutiae_b) {
    /* for complexity reasons, turn the smallest minutiae set to be B */
    if (minutiae_a.size() < minutiae_b.size())
        std::swap(minutiae_a, minutiae_b);

    double similarity = std::numeric_limits<double>::max();

    const int len_b = minutiae_b.size();
    for (int j = 0; j < len_b; j++)
        similarity = std::min(similarity, compute_similarity(j, minutiae_a, minutiae_b));

    return similarity;
}
