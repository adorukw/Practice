#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        if (n == 0) {
            cout << "no\n";
            continue;
        }
        if (n == 1) {
            cout << "no\n";
            continue;
        }
        int a = 1, b = 2;
        int res = 0;
        for (int i = 2; i <= n; i++) {
            res = (a + b) % 3;
            a = b;
            b = res;
        }
        cout << (res == 0 ? "yes" : "no") << '\n';
    }

    return 0;
}