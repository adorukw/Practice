#include <iostream>

using namespace std;

int main() {
    int n, m;
    while (cin >> n >> m) {
        bool first = true;
        for (int i = 1; i <= n; i += m) {
            int count = min(m, n - i + 1);
            int start = i * 2;
            int end = 2 * (i + count - 1);
            int avg = (start + end) / 2;
            if (!first) cout << ' ';
            cout << avg;
            first = false;
        }
        cout << '\n';
    }

    return 0;
}