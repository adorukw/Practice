#include <iostream>

using namespace std;

long long f[21], g[21];

int main() {
    int T;
    cin >> T;
    g[1] = 1, g[0] = 0, f[1] = 2;
    for (int i = 2; i < 21; i++) {
        g[i] = 3 * g[i - 1] + 1;
    }
    while (T--) {
        int n;
        cin >> n;
        cout << 2 * g[n - 1] + 2 << endl;
    }
    return 0;
}