#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<long long> dp(51, 0);
    dp[1] = 1;
    dp[2] = 1;
    for (int i = 3; i <= 50; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    int N;
    while (cin >> N) {
        for (int i = 0; i < N; i++) {
            long long a, b;
            cin >> a >> b;
            cout << dp[b - a + 1] << endl;
        }
    }

    return 0;
}