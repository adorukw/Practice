#include <iomanip>
#include <iostream>

using namespace std;

const double PI = 3.1415927;

int main() {
    double r;
    while (cin >> r) {
        double v = PI * r * r * r * 4 / 3;
        cout << fixed << setprecision(3) << v << '\n';
    }

    return 0;
}