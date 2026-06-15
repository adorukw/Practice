#include <iostream>
#include <vector>

using namespace std;

int main() {
    // 预计算所有结果
    vector<long long> dp(10001, 0);
    dp[1] = 2;

    // 使用递推公式：dp[n] = dp[n-1] + 4*n - 3
    for (int i = 2; i <= 10000; i++) {
        dp[i] = dp[i - 1] + 4 * i - 3;
    }

    int C;
    cin >> C;  // 只读取一个C，不是while循环

    for (int i = 0; i < C; i++) {
        int n;
        cin >> n;  // 读取每个测试用例的n
        cout << dp[n] << endl;
    }

    return 0;
}