#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// 修正1：写判断【一个数是否是素数】的函数（素数环需要相邻和为素数）
bool isPrime(int x) {
    if (x < 2) return false; // 素数最小是2
    for (int i = 2; i <= sqrt(x); i++) {
        if (x % i == 0) return false;
    }
    return true;
}

// 修正2：简化DFS参数，仅保留【当前填充位置、结果数组、使用标记数组】
// currIdx：当前要填充的nums下标，nums[0]已固定为1，从currIdx=1开始
void DFS(int currIdx, vector<int>& nums, vector<bool>& used) {
    int n = nums.size();
    // 递归终止条件：填充完所有位置（currIdx == n）
    if (currIdx == n) {
        // 素数环收尾：最后一位和第一位的和也必须是素数
        if (isPrime(nums[currIdx-1] + nums[0])) {
            // 输出结果，按空格分隔
            for (int i = 0; i < n; i++) {
                if (i > 0) cout << " ";
                cout << nums[i];
            }
            cout << endl;
        }
        return;
    }

    // 遍历1~n的所有数字，尝试填充到currIdx位置
    for (int i = 1; i <= n; i++) {
        // 数字i未被使用，且和前一位的和是素数
        if (!used[i] && isPrime(nums[currIdx-1] + i)) {
            used[i] = true;    // 标记为已使用
            nums[currIdx] = i; // 填充到当前位置（无越界）
            DFS(currIdx + 1, nums, used); // 递归填充下一位
            used[i] = false;   // 回溯：取消标记
        }
    }
}

int main() {
    int n;
    int caseNum = 1;
    // 多组输入，按题目要求输出Case X
    while (cin >> n) {
        cout << "Case " << caseNum++ << ":" << endl;
        vector<int> nums(n);    // 存储素数环结果
        // used数组下标直接对应数字1~n，避免i+1的混淆，大小设为n+1
        vector<bool> used(n + 1, false);
        nums[0] = 1;            // 第一个位置固定为1
        used[1] = true;         // 标记1为已使用
        DFS(1, nums, used);     // 从第1个下标开始填充（下标0已填）
        cout << endl;           // 每个case后空一行，符合题目要求
    }
    return 0;
}