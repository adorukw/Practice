#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<long long> dp(51, 0);

    // 初始化
    dp[1] = 3;
    dp[2] = 6;
    dp[3] = 6;  // 注意：不是12

    // 从4开始使用递推公式
    for (int i = 4; i <= 50; i++) {
        dp[i] = dp[i - 1] + 2 * dp[i - 2];
    }

    int n;
    while (cin >> n) {
        cout << dp[n] << endl;
    }

    return 0;
}