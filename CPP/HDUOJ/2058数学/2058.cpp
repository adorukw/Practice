#include <cmath>
#include <iostream>

using namespace std;

int main() {
    int N, M;
    while (cin >> N >> M) {
        if (N == 0 && M == 0) break;

        for (int k = sqrt(2 * M); k >= 1; k--) {
            if ((2 * M) % k != 0) continue;
            int temp = 2 * M / k - k + 1;
            if (temp % 2 != 0) continue;
            int a = temp / 2;
            int b = a + k - 1;
            if (a <= 0 || b > N) continue;
            cout << "[" << a << "," << b << "]" << endl;
        }
        cout << endl;
    }

    return 0;
}