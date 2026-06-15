#include <iostream>

using namespace std;

int main() {
    int N;
    cin >> N;
    for (int block = 0; block < N; block++) {
        int n, m;
        int caseNum = 1;
        while (cin >> n >> m) {
            if (n == 0 && m == 0) break;
            int count = 0;
            for (int a = 1; a < n; a++) {
                for (int b = a + 1; b < n; b++) {
                    long long numerator =
                        (long long)a * a + (long long)b * b + m;
                    long long denominator = (long long)a * b;
                    if (numerator % denominator == 0) count++;
                }
            }
            cout << "Case " << caseNum++ << ": " << count << '\n';
        }
        if (block != N - 1) cout << '\n';
    }

    return 0;
}