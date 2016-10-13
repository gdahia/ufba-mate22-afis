#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#define N_SUBJECTS 50
#define N_IMGS 5
#define RESULT_PATH "result/human_performance/"
using namespace std;
using namespace cv;
int MAX_SQR_DIST;

int sqr_dist(const Point & p1, const Point & p2) {
    return ((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

struct Comp {
    Point p;
    Comp(Point p) : p(p) {}
    bool operator()(const Point & a, const Point & b) {
        return sqr_dist(a, p) < sqr_dist(b, p);
    }
};

bool vfind(const vector<Point> & v, const Point & q) {
    for (const Point & p : v)
        if (p == q) return true;
    return false;
}

Point find_best_match(const Point p, vector<Point> qry, const vector<Point> & disc) {
    if (qry.empty()) {
        std::cerr << "Erro: vetor de pontos vazio" << std::endl;
        exit(1);
    }

    /* sort by distance from p */
    stable_sort(qry.begin(), qry.end(), Comp(p));

    /* find first usable closest point */
    for (Point q : qry) {
        if (sqr_dist(q, p) > MAX_SQR_DIST)
            return Point(-1, -1);
        if (!vfind(disc, q))
            return q;
    }

    return Point(-1, -1);
}

void compute_distances(const string & ref_path, const string & mrk_path, double & fdr, double & fndr) {
    /* v[0] are endlines and v[1] are bifurcations */
    vector<Point> ref, mrk;

    /* try and read contents of reference file, no duplicates allowed */
    ifstream ref_file(ref_path, ios::in);
    if (!ref_file.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir o arquivo " << ref_path << std::endl;
        exit(1);
    }
    int x, y;
    char t;
    while (ref_file >> x >> y >> t) {
        if (!vfind(ref, Point(x, y)))
            ref.emplace_back(x, y);
    }

    /* try and read contents of markings file, no duplicates allowed */
    ifstream mrk_file(mrk_path, ios::in);
    if (!mrk_file.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir o arquivo " << mrk_path << std::endl;
        exit(1);
    }
    while (mrk_file >> x >> y) {
        if (!vfind(mrk, Point(x, y)))
            mrk.emplace_back(x + 15, y + 15);
    }

    /* correspondences between reference and markings */
    vector<Point> ref_found, mrk_found;

    /* indicative of whether a given minutia was already used and how many are left */
    bool marked[mrk.size()];
    int remaining = mrk.size(), old_remaining = -1;
    memset(marked, false, sizeof marked);

    /* iteratively try and find unquestionable best matches */
    while (remaining != old_remaining) {
        old_remaining = remaining;
        for (int i = 0, len = mrk.size(); i < len; i++) {
            /* if two minutiae, both unused, one in the markings and another in the reference,
            are reciprocally closest to each other, thats an unquestionable best match */
            if (!marked[i]) {
                Point ith_best_match = find_best_match(mrk[i], ref, ref_found);
                if (ith_best_match.x != -1 && mrk[i] == find_best_match(ith_best_match, mrk, mrk_found)) {
                    /* add the match to the correspondences */
                    ref_found.push_back(ith_best_match);
                    mrk_found.push_back(mrk[i]);

                    /* mark the minutia as used */
                    marked[i] = true;

                    /* decrement the remaining minutiae */
                    remaining--;
                }
            }
        }
    }

    fndr += ((double) ref_found.size() / ref.size());
    fdr += 1.0 - ((double) mrk_found.size() / mrk.size());
}

int main(int argc, char ** argv) {
    if (argc <= 2) {
        std::cerr << "Erro: argumentos insuficientes. Passar pasta da base de referencia e base de marcacoes como parametro" << std::endl;
        return 1;
    }

    const string ref = argv[1], mrk = argv[2];

    ofstream result(RESULT_PATH + ref + "-" + mrk + ".txt", ios::out);
    /* open results output file */
    if (!result.is_open()) {
        std::cerr << "Erro: nao foi possivel criar/abrir o arquivo " << RESULT_PATH + ref + "-" + mrk + ".txt" << std::endl;
        return 1;
    }
    /* compute distances for all images */
    for (int dist = 0; dist <= 80; dist++) {
        MAX_SQR_DIST = dist*dist;

        double fdr = 0, fndr = 0;
        for (int i = 1; i <= N_SUBJECTS; i++) {
            for (int j = 1; j <= 5; j++) {
                string ref_path(ref + "/" + to_string(i) + "_" + to_string(j) + ".txt"), mrk_path(mrk + "/" + to_string(i) + "_" + to_string(j) + ".txt");
                compute_distances(ref_path, mrk_path, fdr, fndr);
            }
        }

        result << "thr: " << dist << endl;

        /* output fdr */
        result << "fdr: " << fdr / 250.0 << endl;

        /* output fndr */
        result << "fndr: " << fndr / 250.0 << endl;
        result << endl;
    }
}
