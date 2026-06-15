#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    int m;
    while (cin >> m) {
        for (int i = 0; i < m; i++) {
            int n;
            int sign = 1;
            cin >> n;
            double sum = 0;
            for (int j = 1; j <= n; j++) {
                sum += sign * 1.0 / j;
                sign *= -1;
            }
            cout << fixed << setprecision(2) << sum << '\n';
        }
    }

    return 0;
}