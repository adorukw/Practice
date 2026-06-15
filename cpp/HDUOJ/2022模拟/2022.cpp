#include <climits>
#include <cmath>
#include <iostream>

using namespace std;

int main() {
    int m, n;
    while(cin >> m >> n) {
        int maxAbs = INT_MIN;
        int x, y, s;
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                int temp;
                cin >> temp;
                if (abs(temp) > maxAbs) {
                    x = i;
                    y = j;
                    maxAbs = abs(temp);
                    s = temp;
                }
            }
        }
        cout << x << " " << y << " " << s << '\n';
    }
    return 0;
}