#include <iostream>
#include <vector>

using namespace std;

int Gcd(int a, int b) { return b == 0 ? a : Gcd(b, a % b); }

int Lcm(int a, int b) { return a / Gcd(a, b) * b; }

int main() {
    int n;
    while (cin >> n) {
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        int res = 1;
        for (int i = 0; i < n; i++) {
            res = Lcm(res, a[i]);
        }
        cout << res << '\n';
    }

    return 0;
}