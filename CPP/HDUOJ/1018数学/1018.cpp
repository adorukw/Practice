#include <cmath>
#include <iostream>

using namespace std;

int FactorialDigits(int n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;

    double digits = 0;
    for (int i = 2; i <= n; i++) digits += log10(i);

    return (int)digits + 1;
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        cout << FactorialDigits(num) << '\n';
    }

    return 0;
}