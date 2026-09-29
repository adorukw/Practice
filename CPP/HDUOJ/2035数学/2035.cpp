#include <iostream>

using namespace std;

int main() {
    int A, B;
    while (cin >> A >> B) {
        if (A == 0 && B == 0) break;
        int res = 1;

        for (int i = 0; i < B; i++) {
            res *= A;
            res %= 1000;
        }
        cout << res << endl;
    }

    return 0;
}