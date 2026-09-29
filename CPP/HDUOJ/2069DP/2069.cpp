#include <iostream>
#include <cstring>
using namespace std;

const int MAX_AMOUNT = 251;  // 金额最大250
const int MAX_COINS = 101;   // 硬币最多100个
const int coins[5] = {1, 5, 10, 25, 50};

long long dp[MAX_AMOUNT][MAX_COINS];  // dp[i][j]: 凑出i美分用j个硬币的方法数

void preprocess() {
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;  // 0美分用0个硬币有1种方法
    
    // 遍历所有硬币
    for (int c = 0; c < 5; c++) {
        int coin = coins[c];
        
        // 遍历金额
        for (int amount = coin; amount < MAX_AMOUNT; amount++) {
            // 遍历硬币数量
            for (int count = 1; count < MAX_COINS; count++) {
                dp[amount][count] += dp[amount - coin][count - 1];
            }
        }
    }
}

int main() {
    preprocess();  // 预处理所有可能
    
    int amount;
    while (cin >> amount) {
        long long ways = 0;
        
        // 累加使用1到100个硬币的方法数
        for (int count = 0; count < MAX_COINS; count++) {
            ways += dp[amount][count];
        }
        
        cout << ways << endl;
    }
    
    return 0;
}