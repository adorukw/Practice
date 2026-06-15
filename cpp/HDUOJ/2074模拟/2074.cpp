#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    char center, margin;
    int T = 0;
    while (cin >> n >> center >> margin) {
        // 多个测试用例之间输出空行
        if (T > 0) cout << endl;
        
        // 特判 n == 1 的情况
        if (n == 1) {
            cout << center << endl;
            T++; // 修复：必须增加 T 的计数，否则后续空行逻辑会乱
            continue;
        }

        // 修复：判断从中心到最外层的圈数。如果是偶数圈，最外层需要使用中心花色
        if ((n / 2) % 2 == 0) {
            char temp = center;
            center = margin;
            margin = temp;
        }
        
        vector<vector<char>> res(n, vector<char>(n, ' '));

        // 画最外层
        for (int i = 0; i < n; i++) {
            res[0][i] = margin;
            res[n - 1][i] = margin;
            res[i][0] = margin;
            res[i][n - 1] = margin;
        }
        
        // 打磨最外层的四个角
        res[0][0] = ' ';
        res[0][n - 1] = ' ';
        res[n - 1][0] = ' ';
        res[n - 1][n - 1] = ' ';

        // 从第二层开始向内画（i <= n / 2 即可，再往内没有意义了）
        for (int i = 1; i <= n / 2; i++) {
            if (i % 2 == 1) {
                for (int j = i; j < n - i; j++) {
                    res[i][j] = center;
                    res[n - 1 - i][j] = center;
                    res[j][i] = center;
                    res[j][n - 1 - i] = center;
                }
            }
            else {
                for (int j = i; j < n - i; j++) {
                    res[i][j] = margin;
                    res[n - 1 - i][j] = margin;
                    res[j][i] = margin;
                    res[j][n - 1 - i] = margin;
                }
            }
        }

        // 输出结果
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << res[i][j];
            }
            cout << endl;
        }

        T++;
    }
    return 0;
}