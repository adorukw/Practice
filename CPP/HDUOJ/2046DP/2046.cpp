#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<long long> dp(51, 0);
    dp[1] = 1;
    dp[2] = 2;
    for (int i = 3; i <= 50; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    int n;
    while (cin >> n) {
        cout << dp[n] << endl;
    }

    return 0;
}