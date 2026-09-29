#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

const double PI = acos(-1.0);

int main() {
    int T;
    cin >> T;
    while (T--) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        double cosDegree = (x1 * x2 + y1 * y2) /
                           (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2));
        double degree = acos(cosDegree) * 180 / PI;
        cout << fixed << setprecision(2) << degree << endl;
    }
    return 0;
}