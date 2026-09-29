#include <algorithm>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    double x1, y1, x2, y2, x3, y3, x4, y4;
    while (cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4) {
        double l1 = min(x1, x2);
        double r1 = max(x1, x2);
        double d1 = min(y1, y2);
        double u1 = max(y1, y2);

        double l2 = min(x3, x4);
        double r2 = max(x3, x4);
        double d2 = min(y3, y4);
        double u2 = max(y3, y4);

        double l = max(l1, l2);
        double r = min(r1, r2);
        double d = max(d1, d2);
        double u = min(u1, u2);

        double w = r - l, h = u - d;
        double area = 0.0;
        if (w > 0 && h > 0) area = w * h;
        cout << fixed << setprecision(2) << area << endl;
    }

    return 0;
}