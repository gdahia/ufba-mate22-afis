#include <bits/stdc++.h>

using namespace std;

int main(int argc, char ** argv) {
    if (argc <= 2)
        return 1;
    ifstream genuine_file(argv[1]);
    std::vector<double> genuine_scores;
    set<double> all_scores;
    double a;
    while (genuine_file >> a) {
        genuine_scores.push_back(a);
        all_scores.insert(a);
    }
    ifstream impostor_file(argv[2]);
    std::vector<double> impostor_scores;
    while (impostor_file >> a) {
        impostor_scores.push_back(a);
        all_scores.insert(a);
    }
    sort(genuine_scores.begin(), genuine_scores.end());
    sort(impostor_scores.begin(), impostor_scores.end());
    int imp = 0, gen = 0;
    const int gen_size = genuine_scores.size();
    const int imp_size = impostor_scores.size();
    ofstream frr("frr.txt"), far("far.txt");
    for (double score = 0.0; score <= 1.0; score += 0.01) {
        while (gen < gen_size && genuine_scores[gen] <= score)
            gen++;
        while (imp < imp_size && impostor_scores[imp] <= score)
            imp++;
        frr << fixed << (double) gen / gen_size << endl;
        far << fixed << 1.0 - ((double) imp / imp_size) << endl;
    }
}
