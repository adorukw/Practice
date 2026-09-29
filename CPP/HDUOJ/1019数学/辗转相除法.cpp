#include <iostream>
#include <vector>
using namespace std;

long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        long long ans = 1, x;
        for (int i = 0; i < n; i++) {
            cin >> x;
            ans = lcm(ans, x);
        }
        cout << ans << '\n';
    }
    
    return 0;
}