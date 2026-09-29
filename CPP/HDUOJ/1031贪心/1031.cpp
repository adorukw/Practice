#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N, M, K;
    while (cin >> N >> M >> K) {
        vector<pair<double, int>> score(M);
        for (int j = 0; j < M; j++) {
            score[j].first = 0.0;
            score[j].second = j + 1;
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                double currScore;
                cin >> currScore;
                score[j].first += currScore;
            }
        }
        sort(score.begin(), score.end(),
             [](const pair<double, int>& a, const pair<double, int>& b) {
                 if (a.first != b.first) {
                     return a.first > b.first;
                 }
                 return a.second < b.second;
             });
        vector<int> indices(K);
        for (int i = 0; i < K; i++) {
            indices[i] = score[i].second;
        }

        sort(indices.rbegin(), indices.rend());

        cout << indices[0];
        for (int i = 1; i < K; i++) {
            cout << ' ' << indices[i];
        }
        cout << '\n';
    }

    return 0;
}