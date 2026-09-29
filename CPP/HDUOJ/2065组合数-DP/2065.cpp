#include <iostream>

using namespace std;

int PowMod(int a, unsigned long long b, int mod) {
    int res = 1 % mod;
    while (b > 0) {
        if (b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

int main() {
    int T;
    while (cin >> T) {
        if (T == 0) break;
        for (int i = 1; i <= T; i++) {
            unsigned long long N;
            cin >> N;
            int res = 0;
            if (N == 1)
                res = 2;
            else {
                int a = PowMod(4, N - 1, 100);
                int b = PowMod(2, N - 1, 100);
                res = (a + b) % 100;
            }
            cout << "Case " << i << ": " << res << endl;
        }
        cout << endl;
    }
    return 0;
}