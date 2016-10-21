#include <bits/stdc++.h>

using namespace std;

int main(int argc, char ** argv) {
    if (argc <= 2)
        return 1;
    ifstream genuine_file(argv[1]);
    std::vector<int> genuine_scores;
    int a, max_val = 0, gen_discarded = 0;
    while (genuine_file >> a) {
        if (!a) {
            gen_discarded++;
            continue;
        }
        genuine_scores.push_back(a);
        max_val = max(a, max_val);
    }
    ifstream impostor_file(argv[2]);
    std::vector<int> impostor_scores;
    int imp_discarded = 0;
    while (impostor_file >> a) {
        if (!a) {
            imp_discarded++;
            continue;
        }
        impostor_scores.push_back(a);
        max_val = max(a, max_val);
    }
    cout << gen_discarded << " " << imp_discarded << endl;
    sort(genuine_scores.begin(), genuine_scores.end());
    sort(impostor_scores.begin(), impostor_scores.end());
    int imp = 0, gen = 0;
    const int gen_size = genuine_scores.size();
    const int imp_size = impostor_scores.size();
    ofstream frr("frr.txt"), far("far.txt");
    for (int score = 0; score < max_val; score++) {
        while (gen < gen_size && genuine_scores[gen] <= score)
            gen++;
        while (imp < imp_size && impostor_scores[imp] <= score)
            imp++;
        frr << (double) gen / gen_size << endl;
        far << 1.0 - ((double) imp / imp_size) << endl;
    }
}
