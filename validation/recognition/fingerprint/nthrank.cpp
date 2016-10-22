#include <bits/stdc++.h>

using namespace std;

unsigned int nth_rank(vector<pair<int, int>> & sims, const unsigned int gen) {
    sort(sims.begin(), sims.end(), greater<pair<int, int>>());

    queue<int> q;
    bool appeared_class[51];
    memset(appeared_class, false, sizeof appeared_class);
    for (pair<int, int> p : sims) {
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
    vector<pair<int, int>> sims[51];
    int sub1, reg1, sub2, reg2, sim;
    while (input >> sub1 >> reg1 >> sub2 >> reg2 >> sim)
        sims[sub1].emplace_back(sim, sub2);


    /* compute ranks */
    unsigned int ranks[51];
    memset(ranks, 0, sizeof ranks);

    for (int i = 1; i <= 50; i++)
        ranks[nth_rank(sims[i], i)]++;

    for (int i = 2; i <= 50; i++)
        ranks[i] += ranks[i - 1];

    /* output ranks in R format and skip repetitions */
    int last = 0;
    for (int i = 1; i <= 50; i++) {
        if (ranks[i] != last) {
            last = ranks[i];
            while (ranks[i]--)
                cout << i << endl;
        }
    }
}
