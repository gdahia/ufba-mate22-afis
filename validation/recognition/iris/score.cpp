#include <bits/stdc++.h>

using namespace std;

int main(int argc, char ** argv) {
    if (argc <= 2)
        return 1;
    ifstream genuine_file(argv[1]);
    std::vector<double> genuine_scores, all_scores;
    double a;
    while (genuine_file >> a) {
        genuine_scores.push_back(a);
        all_scores.push_back(a);
    }
    ifstream impostor_file(argv[2]);
    std::vector<double> impostor_scores;
    while (impostor_file >> a) {
        impostor_scores.push_back(a);
        all_scores.push_back(a);
    }
    sort(genuine_scores.rbegin(), genuine_scores.rend());
    sort(impostor_scores.rbegin(), impostor_scores.rend());
    sort(all_scores.rbegin(), all_scores.rend());
    int imp = 0, gen = 0;
    const int gen_size = genuine_scores.size();
    const int imp_size = impostor_scores.size();
    ofstream frr("frr.txt"), far("far.txt");
    for (int i = 0, len = all_scores.size(); i < len; i++) {
        while (gen < gen_size && genuine_scores[gen] > all_scores[i])
            gen++;
        while (imp < imp_size && impostor_scores[imp] > all_scores[i])
            imp++;
        frr << fixed << (double) gen / gen_size << endl;
        far << fixed << 1.0 - ((double) imp / imp_size) << endl;
    }
}
