#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// 计算从原点 (0,0) 沿折线走到目标点 (x, y) 的总距离
double get_dist(int x, int y) {
    int S = x + y;
    double total_dist = 0.0;
    const double sqrt2 = sqrt(2.0); // 提前计算好根号2

    // 累加前 S-1 层的完整距离以及跨层跳跃的距离
    for (int k = 0; k < S; ++k) {
        // k * sqrt2 是第 k 层内的斜线总长
        // sqrt(k*k + (k+1)*(k+1)) 是从第 k 层终点 (k,0) 跳到第 k+1 层起点 (0,k+1) 的直线路程
        total_dist += k * sqrt2 + sqrt(k * k + (k + 1) * (k + 1));
    }

    // 加上在当前第 S 层内部，从 (0,S) 走到 (x,y) 的斜线距离
    total_dist += x * sqrt2;

    return total_dist;
}

int main() {
    // 优化输入输出流，加速执行
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (cin >> N) {
        while (N--) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;

            // 两点之间的距离等于它们各自到原点距离之差的绝对值
            double ans = abs(get_dist(x1, y1) - get_dist(x2, y2));

            // 控制输出格式，保留三位小数
            cout << fixed << setprecision(3) << ans << "\n";
        }
    }

    return 0;
}