#include <bits/stdc++.h>

using namespace std;

int main() {
    int sub1, sub2, d;
    double score;
    while (cin >> sub1 >> d >> sub2 >> d >> score)
        (sub1 == sub2 ? cout : cerr) << score << endl;
    return 0;
}
