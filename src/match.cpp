#include <afis.hpp>
#include <limits>
#include <algorithm>

#ifndef MAX_DIST
#define MAX_DIST 64
#endif

struct Translator_Rotator {
    const double rot_cos, rot_sin;
    const cv::Point center, translation;

    Translator_Rotator(cv::Point center, cv::Point translation, double argcos, double argsin) : translation(translation), center(center), rot_cos(argcos), rot_sin(argsin) {}

    cv::Point2d operator()(cv::Point p) {
        p += translation;
        return cv::Point2d((p.x * rot_cos) + (p.y * rot_sin) + ((1.0 - rot_cos) * center.x) - (rot_sin * center.y), (- p.x * rot_sin) + (p.y * rot_cos) + (rot_sin * center.x) + ((1 - rot_cos) * center.y));
    }
};

template<typename QueryType, typename LookupType>
int find_best_match(const cv::Point_<QueryType> qry, const std::vector<cv::Point_<LookupType>> & lookup, bool * marked) {
    if (lookup.empty()) {
        std::cerr << "Erro: vetor de pontos vazio" << std::endl;
        exit(1);
    }

    int ret = -1;
    double min_dist = MAX_DIST;

    /* find first usable closest point */
    for (int i = 0, len = lookup.size(); i < len; i++) {
        if (!marked[i]) {
            double dist = pow(std::hypot((double) lookup[i].x - qry.x, (double) lookup[i].y - qry.y), 2.0);
            if (dist < min_dist) {
                min_dist = dist;
                ret = i;
            }
        }
    }

    return ret;
}

/* computes the most similar possible match assuming idx_b matches idx_a */
int find_similarity(const int idx_a1, const int idx_a2, const int idx_b1, const int idx_b2, const std::vector<cv::Point> & minutiae_a, const std::vector<cv::Point> & minutiae_b) {
    const cv::Point ref_vec = minutiae_a[idx_a1] - minutiae_a[idx_a2], trans = minutiae_a[idx_a1] - minutiae_b[idx_b1];

    const cv::Point vec = minutiae_b[idx_b1] - minutiae_b[idx_b2];
    const double norm_prod = cv::norm(ref_vec) * cv::norm(vec);
    const double rot_cos = ref_vec.ddot(vec) / norm_prod;
    const double rot_sin = ref_vec.cross(vec) / norm_prod;

    std::vector<cv::Point2d> rotated_minutiae_b(minutiae_b.size());
    std::transform(minutiae_b.begin(), minutiae_b.end(), rotated_minutiae_b.begin(), Translator_Rotator(minutiae_a[idx_a1], trans, rot_cos, rot_sin));

    const std::vector<cv::Point> & ref = minutiae_a;
    const std::vector<cv::Point2d> & mrk = rotated_minutiae_b;

    double distance = pow(std::hypot((double) ref[idx_b2].x - mrk[idx_a2].x, (double) ref[idx_b2].y - mrk[idx_a2].y), 2.0);

    /* indicative of whether a given minutia was already used and how many are left */
    bool ref_marked[ref.size()];
    bool mrk_marked[mrk.size()];
    int remaining = ref.size() - 2, old_remaining = -1;
    memset(ref_marked, false, sizeof ref_marked);
    memset(mrk_marked, false, sizeof mrk_marked);
    ref_marked[idx_a1] = ref_marked[idx_a2] = true;
    mrk_marked[idx_b1] = mrk_marked[idx_b2] = true;

    /* iteratively try and find unquestionable best matches */
    while (remaining != old_remaining) {
        old_remaining = remaining;
        for (int i = 0, len = mrk.size(); i < len; i++) {
            /* if two minutiae, both unused, one in the markings and another in the reference,
            are reciprocally closest to each other, thats an unquestionable best match */
            if (!ref_marked[i]) {
                int found = find_best_match<double, int>(mrk[i], ref, ref_marked);
                if (found > 0 && find_best_match<int, double>(ref[found], mrk, mrk_marked) == i) {
                    /* mark the minutia as used */
                    ref_marked[found] = true;
                    mrk_marked[i] = true;

                    distance += pow(std::hypot((double) ref[found].x - mrk[i].x, (double) ref[found].y - mrk[i].y), 2.0);

                    /* decrement the remaining minutiae */
                    remaining--;
                }
            }
        }
    }

    return ref.size() - remaining;
}

/* finds the closest match between two minutiae coordinates */
// double afis::match(const std::vector<cv::Point> & minutiae_a, const std::vector<cv::Point> & minutiae_b) {
int afis::match(const std::vector<cv::Point> & minutiae_a, const std::vector<cv::Point> & minutiae_b) {
    // double similarity = std::numeric_limits<double>::max();
    int matches = 0;

    const int len_b = minutiae_b.size();
    const int len_a = minutiae_a.size();
    for (int idx_a1 = 0; idx_a1 < len_a; idx_a1++)
        for (int idx_a2 = idx_a1 + 1; idx_a2 < len_a; idx_a2++)
            for (int idx_b1 = 0; idx_b1 < len_b; idx_b1++)
                for (int idx_b2 = idx_b1 + 1; idx_b1 < len_b; idx_b1++)
                    // similarity = std::min(similarity, find_similarity(idx_a1, idx_a2, idx_b1, idx_b2, minutiae_a, minutiae_b));
                    matches = std::max(matches, find_similarity(idx_a1, idx_a2, idx_b1, idx_b2, minutiae_a, minutiae_b));

    // return similarity;
    return matches;
}
