#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 50;

int main() {
    int N;
    cin >> N;
    while (N--) {
        vector<long long> dp(MAXN + 1, 0);
        dp[0] = 1;
        for (int i = 1; i <= 26; i++) {
            int count;
            cin >> count;

            for (int j = MAXN; j >= i; j--) {
                for (int k = 1; k <= count; k++) {
                    int prev = j - k * i;
                    if (prev < 0) break;
                    dp[j] += dp[prev];
                }
            }
        }
        long long res = 0;
        for (int i = 1; i <= MAXN; i++) {
            res += dp[i];
        }
        cout << res << endl;
    }
    return 0;
}