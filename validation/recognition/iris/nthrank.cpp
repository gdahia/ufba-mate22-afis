#include <bits/stdc++.h>

using namespace std;

unsigned int nth_rank(vector<pair<double, int>> & diffs, const unsigned int gen) {
    sort(diffs.begin(), diffs.end());

    queue<int> q;
    bool appeared_class[51];
    memset(appeared_class, false, sizeof appeared_class);
    for (pair<double, int> p : diffs) {
        if (p.second == gen) {
            while (!q.empty()) {
                unsigned int tobmarked = q.front();
                q.pop();
                appeared_class[tobmarked] = true;
            }
        }
        q.push(p.second);
    }

    unsigned int rank = 0;
    for (int i = 1; i <= 50; i++)
        rank += appeared_class[i];

    return rank;
}

int main(int argc, char ** argv) {
    if (argc <= 1) {
        cerr << "Error: pass the similarity/difference file as parameter" << endl;
        return 1;
    }

    /* attempt to open file */
    ifstream input(argv[1], ios::in);
    if (!input.is_open()) {
        cerr << "Error: Unable to open file " << argv[1] << endl;
        return 1;
    }

    /* read contents and store accordingly */
    vector<pair<double, int>> diffs[51];
    int sub1, reg1, sub2, reg2;
    double diff;
    while (input >> sub1 >> reg1 >> sub2 >> reg2 >> diff)
        diffs[sub1].emplace_back(diff, sub2);

    unsigned int rank[51];
    memset(rank, 0, sizeof rank);

    /* store ranks */
    for (int i = 1; i <= 50; i++)
        rank[nth_rank(diffs[i], i)]++;

    /* output ranks */
    int final_rank = 0;
    for (int i = 1; i <= 50; i++) {
        final_rank += rank[i];
        cout << final_rank << endl;
    }
}
