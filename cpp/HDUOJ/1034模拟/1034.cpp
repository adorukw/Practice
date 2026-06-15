#include <iostream>
#include <vector>

using namespace std;

bool IsEnd(const vector<int>& candys) {
    int first = candys[0];
    for (int num : candys) {
        if (num != first) return false;
    }
    return true;
}

int main() {
    int N;
    while (cin >> N && N != 0) {
        vector<int> candys(N);
        for (int i = 0; i < N; i++) {
            cin >> candys[i];
        }
        int round = 0;

        while (!IsEnd(candys)) {
            vector<int> give(N);
            for (int i = 0; i < N; i++) give[i] = candys[i] / 2;

            vector<int> temp(N);
            for (int i = 0; i < N; i++)
                temp[i] = candys[i] - give[i] + give[(i - 1 + N) % N];

            for (int i = 0; i < N; i++) {
                if (temp[i] % 2 != 0) temp[i]++;
            }

            candys = temp;
            round++;
        }
        cout << round << ' ' << candys[0] << endl;
    }

    return 0;
}