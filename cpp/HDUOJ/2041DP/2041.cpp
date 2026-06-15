#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> dp(41, 0);
    dp[1] = 1;
    dp[2] = 1;
    for (int i = 3; i <= 40; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    int N;
    while (cin >> N) {
        for (int i = 0; i < N; i++) {
            int M;
            cin >> M;
            cout << dp[M] << endl;
        }
    }

    return 0;
}