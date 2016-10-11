#include <afis.hpp>
#include <limits>
#include <algorithm>

struct Translator_Rotator {
    const double rot_cos, rot_sin;
    const cv::Point center, translation;

    Translator_Rotator(cv::Point center, cv::Point translation, double argcos, double argsin) : translation(translation), center(center), rot_cos(argcos), rot_sin(argsin) {}

    cv::Point2d operator()(cv::Point p) {
        p += translation;
        return cv::Point2d((p.x * rot_cos) + (p.y * rot_sin) + ((1.0 - rot_cos) * center.x) - (rot_sin * center.y), (- p.x * rot_sin) + (p.y * rot_cos) + (rot_sin * center.x) + ((1 - rot_cos) * center.y));
    }
};

template<typename T>
bool vfind(const std::vector<cv::Point_<T>> & v, const cv::Point_<T> & q) {
    for (const cv::Point_<T> & p : v)
        if (p == q) return true;
    return false;
}

template<typename QueryType, typename LookupType>
std::pair<bool, cv::Point_<LookupType>> find_best_match(const cv::Point_<QueryType> qry, const std::vector<cv::Point_<LookupType>> & lookup, const std::vector<cv::Point_<LookupType>> & disc) {
    if (lookup.empty()) {
        std::cerr << "Erro: vetor de pontos vazio" << std::endl;
        exit(1);
    }

    bool ret = false;
    double min_dist = std::numeric_limits<double>::max();
    cv::Point_<LookupType> best_point(0, 0);

    /* find first usable closest point */
    for (cv::Point_<LookupType> p : lookup) {
        if (!vfind<LookupType>(disc, p)) {
            double dist = std::hypot((double) p.x - qry.x, (double) p.y - qry.y);
            if (dist < min_dist) {
                ret = true;
                best_point = p;
                min_dist = dist;
            }
        }
    }

    return std::make_pair(ret, best_point);
}

/* computes the most similar possible match assuming idx_b matches idx_a */
double find_similarity(const int idx_a1, const int idx_a2, const int idx_b1, const int idx_b2, const std::vector<cv::Point> & minutiae_a, const std::vector<cv::Point> & minutiae_b) {

    const cv::Point ref_vec = minutiae_a[idx_a1] - minutiae_a[idx_a2], trans = minutiae_a[idx_a1] - minutiae_b[idx_b1];

    const cv::Point vec = minutiae_b[idx_b1] - minutiae_b[idx_b2];
    const double norm_prod = cv::norm(ref_vec) * cv::norm(vec);
    const double rot_cos = ref_vec.ddot(vec) / norm_prod;
    const double rot_sin = ref_vec.cross(vec) / norm_prod;

    std::vector<cv::Point2d> rotated_minutiae_b(minutiae_b.size());
    std::transform(minutiae_b.begin(), minutiae_b.end(), rotated_minutiae_b.begin(), Translator_Rotator(minutiae_a[idx_a1], trans, rot_cos, rot_sin));

    const std::vector<cv::Point> & ref = minutiae_a;
    const std::vector<cv::Point2d> & mrk = rotated_minutiae_b;

    /* indicative of whether a given minutia was already used and how many are left */
    bool marked[ref.size()];
    int remaining = ref.size(), old_remaining = -1;
    memset(marked, false, sizeof marked);
    marked[idx_a1] = marked[idx_a2] = true;
    std::vector<cv::Point> ref_found;
    std::vector<cv::Point2d> mrk_found;
    double distance = std::hypot(mrk[idx_a2].x - ref[idx_b2].x, mrk[idx_a2].y - ref[idx_b2].y);

    /* iteratively try and find unquestionable best matches */
    while (remaining != old_remaining) {
        old_remaining = remaining;
        for (int i = 0, len = mrk.size(); i < len; i++) {
            /* if two minutiae, both unused, one in the markings and another in the reference,
            are reciprocally closest to each other, thats an unquestionable best match */
            if (!marked[i]) {
                cv::Point ith_best_match;
                bool found;
                std::tie(found, ith_best_match) = find_best_match<double, int>(mrk[i], ref, ref_found);
                if (found) {
                    /* add the match to the correspondences */
                    ref_found.push_back(ith_best_match);
                    mrk_found.push_back(mrk[i]);
                    distance += std::hypot((double) mrk[i].x - ith_best_match.x, (double) mrk[i].y - ith_best_match.y);

                    /* mark the minutia as used */
                    marked[i] = true;

                    /* decrement the remaining minutiae */
                    remaining--;
                }
            }
        }
    }

    return distance;
}

/* finds the closest match between two minutiae coordinates */
double afis::match(const std::vector<cv::Point> & minutiae_a, const std::vector<cv::Point> & minutiae_b) {
    double similarity = std::numeric_limits<double>::max();

    const int len_b = minutiae_b.size();
    const int len_a = minutiae_a.size();
    for (int idx_a1 = 0; idx_a1 < len_a; idx_a1++)
        for (int idx_a2 = 0; idx_a2 < len_a; idx_a2++) if (idx_a1 != idx_a2)
            for (int idx_b1 = 0; idx_b1 < len_b; idx_b1++)
                for (int idx_b2 = 0; idx_b1 < len_b; idx_b1++) if (idx_b1 != idx_b2)
                    similarity = std::min(similarity, find_similarity(idx_a1, idx_a2, idx_b1, idx_b2, minutiae_a, minutiae_b));


    return similarity;
}
