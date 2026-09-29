#include <cstring>
#include <iostream>
using namespace std;

const int MAX_AMOUNT = 40;
long long memo[MAX_AMOUNT];  // 记忆数组

long long Hanoi3(int n, char from, char mid, char to) {
    if (memo[n] != -1) return memo[n];  // 如果已经计算过，直接返回

    if (n == 1) {
        memo[n] = 2;  // 存储结果
        return 2;
    }

    long long cnt = 0;
    cnt += Hanoi3(n - 1, from, to, mid);  // n-1个from移向to
    cnt += 1;  // 第n个移向mid
    cnt += Hanoi3(n - 1, to, mid, from);  // n-1个to移向from
    cnt += 1;  // 第n个移向to
    cnt += Hanoi3(n - 1, from, mid, to);  // n-1个from移向to

    memo[n] = cnt;  // 存储结果
    return cnt;
}

int main() {
    int n;

    // 初始化记忆数组为-1
    memset(memo, -1, sizeof(memo));

    while (cin >> n) {
        cout << Hanoi3(n, 'A', 'B', 'C') << endl;
    }

    return 0;
}