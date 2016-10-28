#include <bits/stdc++.h>

// far of 0.01
#define IRIS_MAX 31.1402
#define FINGERPRINT_MIN 12

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc <= 2) {
        std::cerr << "Error: pass both files as arguments" << std::endl;
        return 1;
    }

    /* attempt to open iris file */
    ifstream input1(argv[1], ios::in);
    if (!input1.is_open()) {
        cerr << "Error: Unable to open file " << argv[1] << endl;
        return 1;
    }

    /* read contents and store accordingly */
    double diffs[51][6][51][6];
    int sub1, reg1, sub2, reg2;
    double diff;
    while (input1 >> sub1 >> reg1 >> sub2 >> reg2 >> diff)
        diffs[sub1][reg1][sub2][reg2] = diff;

    /* attempt to open fingerprint file */
    ifstream input2(argv[2], ios::in);
    if (!input2.is_open()) {
        cerr << "Error: Unable to open file " << argv[2] << endl;
        return 1;
    }

    /* read contents and store accordingly */
    int sims[51][6][51][6];
    int sim;
    while (input2 >> sub1 >> reg1 >> sub2 >> reg2 >> sim)
        sims[sub1][reg1][sub2][reg2] = sim;

    /* compute freqs for last 25 elements */
    unsigned int p[8];
    memset(p, 0, sizeof p);
    for (int sub1 = 26; sub1 <= 50; sub1++)
        for (int sub2 = 26; sub2 <= 50; sub2++)
            for (int reg1 = 1; reg1 <= 5; reg1++)
                for (int reg2 = 1; reg2 <= 5; reg2++)
                    if (sub1 != sub2 || reg1 != reg2) {
                        const unsigned int msk = (sub1 == sub2 ? 4 : 0) | (sims[sub1][reg1][sub2][reg2] >= FINGERPRINT_MIN ? 2 : 0) | (diffs[sub1][reg1][sub2][reg2] < IRIS_MAX ? 1 : 0);
                        p[msk]++;
                    }

    /* output result for first 25 elements */
    for (int sub1 = 1; sub1 <= 25; sub1++)
        for (int sub2 = 1; sub2 <= 25; sub2++)
            for (int reg1 = 1; reg1 <= 5; reg1++)
                for (int reg2 = 1; reg2 <= 5; reg2++)
                    if (sub1 != sub2 || reg1 != reg2) {
                        const unsigned int msk = (sims[sub1][reg1][sub2][reg2] >= FINGERPRINT_MIN ? 2 : 0) | (diffs[sub1][reg1][sub2][reg2] < IRIS_MAX ? 1 : 0);
                        cout << sub1 << " " << reg1 << " " << sub2 << " " << reg2 << endl;
                        cout << fixed << (double) p[(1 << 2) | msk] / (p[msk] + p[(1 << 2) | msk]) << endl;
                    }

    /* compute freqs for first 25 elements */
    memset(p, 0, sizeof p);
    for (int sub1 = 1; sub1 <= 25; sub1++)
        for (int sub2 = 1; sub2 <= 25; sub2++)
            for (int reg1 = 1; reg1 <= 5; reg1++)
                for (int reg2 = 1; reg2 <= 5; reg2++)
                    if (sub1 != sub2 || reg1 != reg2) {
                        const unsigned int msk = (sub1 == sub2 ? 4 : 0) | (sims[sub1][reg1][sub2][reg2] >= FINGERPRINT_MIN ? 2 : 0) | (diffs[sub1][reg1][sub2][reg2] < IRIS_MAX ? 1 : 0);
                        p[msk]++;
                    }

    /* output result for last 25 elements */
    for (int sub1 = 25; sub1 <= 50; sub1++)
        for (int sub2 = 25; sub2 <= 50; sub2++)
            for (int reg1 = 1; reg1 <= 5; reg1++)
                for (int reg2 = 1; reg2 <= 5; reg2++)
                    if (sub1 != sub2 || reg1 != reg2) {
                        const unsigned int msk = (sims[sub1][reg1][sub2][reg2] >= FINGERPRINT_MIN ? 2 : 0) | (diffs[sub1][reg1][sub2][reg2] < IRIS_MAX ? 1 : 0);
                        cout << sub1 << " " << reg1 << " " << sub2 << " " << reg2 << endl;
                        cout << fixed << (double) p[(1 << 2) | msk] / (p[msk] + p[(1 << 2) | msk]) << endl;
                    }
    return 0;
}
