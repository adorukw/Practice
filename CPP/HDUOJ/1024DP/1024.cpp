#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>

using namespace std;

const int MAX_AMOUNT = 1000010;
const int INF = 0x3f3f3f3f;

int a[MAX_AMOUNT];
int dp[MAX_AMOUNT];
int pre[MAX_AMOUNT];

int main() {
    int m, n;
    while (scanf("%d %d", &m, &n) != EOF) {
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
        }
        memset(dp, 0, sizeof(dp));
        memset(pre, 0, sizeof(pre));

        int maxSum = 0;

        for (int i = 1; i <= m; i++) {
            maxSum = -INF;
            for (int j = i; j <= n; j++) {
                if (j == i)
                    dp[j] = pre[j - 1] + a[j];
                else
                    dp[j] = max(dp[j - 1] + a[j], pre[j - 1] + a[j]);

                pre[j - 1] = maxSum;
                maxSum = max(maxSum, dp[j]);
            }
        }
        printf("%d\n", maxSum);
    }

    return 0;
}