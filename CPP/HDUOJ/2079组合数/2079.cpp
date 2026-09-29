#include <iostream>
#include <vector>

using namespace std;

void Solve() {
    int n, k;
    cin >> n >> k;

    // dp 数组：dp[j] 表示凑到 j 个学分的组合数。
    // 在母函数中，这相当于当前多项式 x^j 项的【系数】。
    vector<long long> dp(n + 1, 0);
    // 边界条件/初始化：
    // 凑 0 个学分有 1 种方案（即一门课都不选）。
    // 在母函数中，这相当于最开始有一个常数项 1 (即 1 * x^0)。
    dp[0] = 1;

    // 第一层循环：遍历每一门课
    // (母函数：遍历要乘进去的每一个多项式)
    // (DP：遍历每一个决策阶段/物品)
    for (int i = 0; i < k; i++) {
        int a, b;
        cin >> a >> b;

        // next_dp 数组：用于保存考虑完当前这门课之后的新状态。
        // 因为我们必须基于上一步的状态来计算，直接在原数组上修改会导致重复累加错误。
        vector<long long> nextDp(n + 1, 0);

        // 第二层循环：遍历当前已经凑到的学分状态
        // (母函数：遍历第一个多项式已存在的每一项 x^j)
        // (DP：遍历上一个阶段的所有有效状态)
        for (int j = 0; j <= n; j++) {
            // 只有当凑齐 j 学分的方案数大于 0 时，才需要基于它向后推导
            if (dp[j] > 0) {
                // 第三层循环：遍历当前这门课可以选几门 (从 0 门到 b 门)
                // (母函数：遍历第二个多项式的每一项 x^{c*a}，并展开相乘)
                // (DP：遍历当前物品的选择策略)
                for (int c = 0; c <= b; c++) {
                    int currCredits = c * a;
                    if (j + currCredits <= n) {
                        // 核心转移方程！
                        // 之前的方案数 dp[j] 原封不动地贡献给新的学分总和 (j +
                        // c * a)
                        // (母函数：系数相乘并合并同类项，因为这门课系数都是
                        // 1，所以就是 dp[j] * 1)
                        nextDp[j + currCredits] += dp[j];  // 更新状态
                    }
                    else {
                        // 剪枝优化：如果学分已经超过了我们要凑的 n，
                        // 后面的 c 更大，学分只会更多，所以直接跳出当前循环。
                        break;  // 跳出当前循环
                    }
                }
            }
        }
        dp = nextDp;
    }
    cout << dp[n] << endl;  // 输出最终结果
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        Solve();
    }
    return 0;
}