#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        if (n == 0) break;
        vector<pair<int, int>> points(n);
        for (int i = 0; i < n; i++) {
            int x, y;
            cin >> x >> y;
            points[i] = make_pair(x, y);
        }
        long long sum = 0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            int x1 = points[i].first, y1 = points[i].second;
            int x2 = points[j].first, y2 = points[j].second;
            long long d1 = x1*y2, d2 = x2*y1;
            sum += d1 - d2;
        }
        double ares = sum * 1.0 / 2;

        if (ares >= 0)
            cout << fixed << setprecision(1) << ares << endl;
        else
            cout << fixed << setprecision(1) << -ares << endl;
    }

    return 0;
}