#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

long long Factorial(int n) {
    if (n == 0) return 1;
    return n * Factorial(n - 1);
}

int main() {
    vector<long long> dp(21, 0);
    dp[2] = 1;
    for (int i = 3; i <= 20; i++) {
        dp[i] = (i - 1) * (dp[i - 1] + dp[i - 2]);
    }

    int C;
    while (cin >> C) {
        for (int i = 0; i < C; i++) {
            int n;
            cin >> n;
            cout << fixed << setprecision(2) << dp[n] * 1.0 / Factorial(n) * 100
                 << '%' << endl;
        }
    }

    return 0;
}